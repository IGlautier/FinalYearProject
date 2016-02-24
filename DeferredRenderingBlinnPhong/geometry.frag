#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColour;

in vec2 texPos;
in vec3 normalFrag;
in vec3 fragPos;

uniform vec3 colour;
uniform float specular;

float linearDepth(float depth) {
	const float NEAR = 0.1f;
	const float FAR = 100.0f; 
	float z = depth * 2.0f - 1.0f;
	return (2.0f * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}


void main() {

	gPosition.xyz = fragPos;
	gPosition.a = linearDepth(gl_FragCoord.z);
	gNormal = normalize(normalFrag);
	gColour.rgb = colour;
	gColour.a = specular;

}