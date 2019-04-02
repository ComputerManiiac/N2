#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_modelspace;
layout(location = 1) in vec3 textColour;
layout(location = 2) in mat4 transformMatrix;

out vec3 outputColour;

void main(){
	outputColour = textColour;
	gl_Position = transformMatrix * vec4(vertexPosition_modelspace, 0.0, 1.0);
}

