#version 330 core

/* Per Vertex Data */
layout(location = 0) in vec3 vertexPosition_modelspace;

/* Instanced Data */
//layout(location = 1) in mat4 transformationMatrix;

out vec2 texCoords;

uniform vec2 textureTileSize;
uniform vec2 textureTileOffset;
uniform mat4 transformationMatrix;
uniform mat4 projection;
uniform mat4 view;

void main(){
	texCoords = vertexPosition_modelspace.xy + vec2(0.5, 0.5);
	texCoords /= 32;
	texCoords *= textureTileSize / 16;
	texCoords += textureTileOffset;


//	texCoords.y = 1.0 - texCoords.y;

	gl_Position = projection * view * transformationMatrix * vec4(vertexPosition_modelspace, 1.0);
}

