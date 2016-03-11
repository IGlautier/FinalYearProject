#version 330 core

in vec2 texPos;
in vec3 Normal;
in vec3 fragPos;

out vec4 colour;

uniform vec3 iColour;
uniform float specular;
uniform vec3 camPosition;
uniform vec3 lightPosition;
uniform samplerCube shadowMap;


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

	float shadow = shadowCalculation(fragPos);

	vec3 lightLevel =  (ambient + (1.0f - shadow) * ((spec + diffuse) * attenuation)) * light; // Not attenuating ambient, very little visible difference
	
	colour = vec4(lightLevel * iColour, 1.0f);
	
} 