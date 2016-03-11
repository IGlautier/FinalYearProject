#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColour;

in vec2 texPos;
in vec3 normalFrag;
in vec3 fragPos;
in vec3 lFragPos;

uniform vec3 colour;
uniform float specular;
uniform vec3 lightPosition;
uniform samplerCube shadowMap;

float linearDepth(float depth) {
	const float NEAR = 0.1f;
	const float FAR = 100.0f; 
	float z = depth * 2.0f - 1.0f;
	return (2.0f * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

float shadowCalculation (vec3 position) {
	
	float shadow = 0.0f;

	
	float samples = 4.0;
	float offset = 0.1;
	vec3 lightDistance = position - lightPosition;
	float depth = length(lightDistance);
	for(float x = -2*offset; x < 2*offset; x += offset / (samples * 0.25))
	{
		for(float y = -2*offset; y < 2*offset; y += offset / (samples * 0.25))
		{
			for(float z = -2*offset; z < 2*offset; z += offset / (samples * 0.25))
			{
				float closestDepth = texture(shadowMap, lightDistance + vec3(x, y, z)).r; 
				closestDepth *= 100.0f;   // Undo mapping [0;1]
				if(depth - 0.1f > closestDepth)
					shadow += 1.0;
			}
		}
	}
	shadow /= (samples * samples * samples);

	return shadow;
	


}




void main() {

	gPosition.xyz = fragPos;
	gPosition.a = linearDepth(gl_FragCoord.z);
	gNormal = normalize(normalFrag);
	gColour.rgb = colour;
	gColour.a = shadowCalculation(lFragPos);

}