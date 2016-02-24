#version 330 core

in vec2 texPos;
in vec3 Normal;
in vec3 fragPos;

out vec4 colour;

uniform vec3 iColour;
uniform float specular;
uniform vec3 camPosition;
uniform vec3 lightPosition;


void main()
{
	
	vec3 normal = normalize(Normal);
	
	vec3 lightDir = normalize(lightPosition - fragPos);
	vec3 light = vec3(1.0f, 1.0f, 1.0f);
	
	float ambient = 0.1f;

	float diffuse = max(dot(normal, lightDir), 0.0f);

	vec3 camDir = normalize(camPosition - fragPos);
	vec3 halfVec = normalize(lightDir + camDir);
	float spec = pow(max(dot(normal, halfVec), 0.0f), 32.0f) * specular;

	float distance = length(lightPosition - fragPos);
	float attenuation = 1.0f / (1.0f + 0.022f * distance + 0.0019f * distance * distance); // Light casts to approx 200m

	vec3 lightLevel =  (ambient + ((spec + diffuse) * attenuation)) * light; // Not attenuating ambient, very little visible difference
	
	colour = vec4(lightLevel * iColour, 1.0f);
	
} 