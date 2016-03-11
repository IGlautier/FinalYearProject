#version 330 core

in vec4 fragPos;

uniform vec3 lightPosition;

void main() {

	float lightDistance = length(fragPos.xyz - lightPosition);

	lightDistance = lightDistance / 100.0f; // 100 being far plane value


	gl_FragDepth = lightDistance;
}