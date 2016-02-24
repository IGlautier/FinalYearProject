#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 iTexture;

out vec2 texPos;
out vec3 normalFrag;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 viewSpace = view * model * vec4(position, 1.0f);
	fragPos = viewSpace.xyz;
    gl_Position = projection * viewSpace;
	texPos = iTexture;	
	normalFrag = transpose(inverse(mat3(view * model))) * normal;
}