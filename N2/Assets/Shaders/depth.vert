#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 3) in mat4 model;

uniform mat4 lightProjectionView;

void main(){

	gl_Position = lightProjectionView * model * vec4(vertexPosition_modelspace, 1.0);
}

