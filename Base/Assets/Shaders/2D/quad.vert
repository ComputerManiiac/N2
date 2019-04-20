#version 330 core

/* Per Vertex Data */
layout(location = 0) in vec3 vertexPosition_modelspace;

/* Instanced Data */
//layout(location = 1) in mat4 transformationMatrix;


uniform mat4 transformationMatrix;
uniform mat4 projection;

void main(){
//	texCoords = vertexPosition_modelspace.xy + vec2(0.5, 0.5);
	gl_Position = projection * transformationMatrix * vec4(vertexPosition_modelspace, 1.0);
}

