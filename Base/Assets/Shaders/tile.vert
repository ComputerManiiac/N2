#version 330 core

/* Per Vertex Data */
layout(location = 0) in vec2 vertexPosition_modelspace;

/* Instanced Data */
layout(location = 1) in mat4 transformationMatrix;

out vec3 outputColor;
out vec2 texCoords;

void main(){
	texCoords = vertexPosition_modelspace + vec2(0.5, 0.5);
	gl_Position = transformationMatrix * vec4(vertexPosition_modelspace, 0.0, 1.0);
}

