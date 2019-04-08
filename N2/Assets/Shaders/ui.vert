#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexTexCoords_modelspace;

out vec3 outputColor;
out vec2 texCoords;

void main(){
	texCoords = vertexTexCoords_modelspace;
	gl_Position = vec4(vertexPosition_modelspace, 0.0, 1.0);
}

