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
uniform samplerCube shadowMap;
uniform vec3 lightPosition;

float linearDepth(float depth) {
	const float NEAR = 0.1f;
	const float FAR = 100.0f; 
	float z = depth * 2.0f - 1.0f;
	return (2.0f * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

float shadowCalculation (vec3 position) {
	
	vec3 lightDistance = position - lightPosition;
	float lightDepth = texture(shadowMap, lightDistance).r;
	lightDepth *= 100.0f;
	float depth = length(lightDistance);
	float shadow = 0.0f;
	if ((depth - 0.1f) > lightDepth) shadow = 1.0f;
	
	return shadow;

}

void main() {

	gPosition.xyz = fragPos;
	gPosition.a = linearDepth(gl_FragCoord.z);
	gNormal.xyz = normalize(normalFrag);
//	gNormal.a = shadowCalculation(lFragPos);
	//gNormal.a = 0.5f;
	gColour.rgb = colour;
	gColour.a = shadowCalculation(lFragPos);

}