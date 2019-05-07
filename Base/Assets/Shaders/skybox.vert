#version 330 core
layout (location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in mat4 model;

out vec2 texCoords;
out float fogVisibility;


uniform mat4 viewMatrix;
uniform mat4 projection;

const float fogDensity = 0.0035;
const float fogGradient = 5.0;

void main()
{
	texCoords = vertexTexCoord;

	
	vec3 vertexPosition_worldspace = (model *  vec4(vertexPosition_modelspace, 1.0)).xyz;

	float distFromCamera = vertexPosition_worldspace.y + 300.0f;
	fogVisibility = exp(-pow(distFromCamera * fogDensity, fogGradient));
//	fogVisibility = clamp(1.4 - fogVisibility, 0.0, 1.0);
	fogVisibility = 1.0 - fogVisibility;
	fogVisibility = max(fogVisibility, 0.0);
//	fogVisibility = 1.0;
//
    gl_Position = projection * viewMatrix * model * vec4(vertexPosition_modelspace, 1.0);
}  