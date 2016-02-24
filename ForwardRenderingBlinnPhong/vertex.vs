#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 iTexPos;

out vec2 texPos;
out vec3 fragPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
	Normal = mat3(transpose(inverse(model))) * normal;
	fragPos = vec3(model * vec4(position, 1.0f));
    gl_Position = projection * view * model * vec4(position, 1.0f);
	texPos = iTexPos;
}