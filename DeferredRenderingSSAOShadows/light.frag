#version 330 core

out vec4 colour;
in vec2 texPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColour;
uniform sampler2D ssao;
uniform bool ssaoEnable;
uniform vec3 camPosition;
uniform vec3 lightPosition;


void main()
{

	vec3 fragPos = texture(gPosition, texPos).rgb;
	vec3 normal = texture(gNormal, texPos).rgb;
	float shadow = texture(gColour, texPos).a;
	vec3 fragColour = texture(gColour, texPos).rgb;

	vec3 lightDir = normalize(lightPosition - fragPos);
	vec3 light = vec3(1.0f, 1.0f, 1.0f);


	float ambient = 0.1f;

	if (ssaoEnable) ambient = 0.1f * texture(ssao, texPos).r;
	

	float diffuse = max(dot(normal, lightDir), 0.0f);

	vec3 camDir = normalize(camPosition - fragPos);
	vec3 halfVec = normalize(lightDir + camDir);

	//float specular = pow(max(dot(normal, halfVec), 0.0f), 32.0f) * texture(gColour, texPos).a;
	float specular = pow(max(dot(normal, halfVec), 0.0f), 32.0f) * 0.5f;

	float distance = length(lightPosition - fragPos);
	float attenuation = 1.0f / (1.0f + 0.022f * distance + 0.0019f * distance * distance); // Light casts to approx 200m

	//vec3 lightLevel = (ambient + specular + diffuse) * attenuation * light; // Attenuating ambient



	vec3 lightLevel =  (ambient + (1.0f - shadow) * ((specular + diffuse) * attenuation)) * light; // Not attenuating ambient, very little visible difference
	
	
	colour = vec4(lightLevel * fragColour, 1.0f);
	
} 