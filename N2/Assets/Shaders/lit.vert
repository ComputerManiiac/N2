#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in mat4 model;
layout(location = 7) in mat4 viewMatrix;
layout(location = 11) in mat4 projection;

// Output data ; will be interpolated for each fragment.
out vec3 vertexPosition_cameraspace;
out vec3 vertexNormal_cameraspace;
out vec2 texCoord;
out mat4 view;
out vec4 vertexPosition_lightspace;

uniform mat4 lightProjectionView;
uniform bool lightEnabled;

void main(){

	mat4 MV = viewMatrix * model;
	mat4 MVP = projection * MV;
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

	// Vector position, in camera space
	vertexPosition_cameraspace = ( MV * vec4(vertexPosition_modelspace, 1) ).xyz;
	vertexPosition_lightspace = lightProjectionView * model * vec4(vertexPosition_modelspace, 1.0);
	
	view = viewMatrix;

	if(lightEnabled == true)
	{
		mat4 MV_inverse_transpose = transpose(inverse(MV));
		// Vertex normal, in camera space
		// Use MV if ModelMatrix does not scale the model ! Use its inverse transpose otherwise.
		vertexNormal_cameraspace = ( MV_inverse_transpose * vec4(vertexNormal_modelspace, 0) ).xyz;
	}
	// A simple pass through. The texCoord of each fragment will be interpolated from texCoord of each vertex
	texCoord = vertexTexCoord;
}

