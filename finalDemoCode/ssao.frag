#version 330 core

in vec2 texPos;

out float fragColour;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D noiseTex;
uniform vec3 kernel[64];
uniform mat4 projection;
uniform int ssaoLevel;
uniform float ssaoRadius;

// Used to tile a 4x4 texture over entire screen
const vec2 NOISESCALE = vec2(270.0f, 180.0f); // Screen aspect 1080/720


void main() {

	vec3 fragPos = texture(gPosition, texPos).xyz;
	vec3 normal = texture(gNormal, texPos).rgb;
	vec3 random = texture(noiseTex, texPos * NOISESCALE).xyz;

	vec3 tangent = normalize(random - normal * dot(random, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal); // This can be used to convert a sample location to a view space vector, calculated by the gramm-schmidt process 
	// (don't understand the mathematics of this process but it seems to create an orthonormal axis from arbitary 3 vectors)

	float occlusion = 0.0f;
	for (int i = 0; i < 64; i += ssaoLevel) { // Changing 64 to a lower value reduces number of samples and increases performance

		vec3 sample = TBN * kernel[i];
		sample = fragPos + sample * ssaoRadius; // Can change the value of 1.0f to set radius of the SSAO effect

		vec4 offset = vec4(sample, 1.0f); 
		offset = projection * offset; // Convert view space to screen space
		offset.xyz /= offset.w; // Normalised coordinates
		offset.xyz = offset.xyz * 0.5f + 0.5f; // Shift to small range 0-1

		float sampleDepth = -texture(gPosition, offset.xy).w; // Sample linear depth buffer for depth value of random sample
		float rangeCheck = smoothstep(0.0f, 1.0f, ssaoRadius / abs(fragPos.z - sampleDepth));
		if (sampleDepth > sample.z) occlusion += 1.0f * rangeCheck;
		
	}
	occlusion /= 64.0f/ssaoLevel; // Average value
	fragColour = 1.0f - occlusion;

}
