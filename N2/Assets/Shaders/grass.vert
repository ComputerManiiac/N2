#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

//uniform mat4 projection;
//uniform mat4 view;
uniform mat4 MVP;

void main()
{
	vec3 actualPosition = -aPos;
    texCoords = actualPosition;
    gl_Position = MVP * vec4(actualPosition, 1.0);
}  