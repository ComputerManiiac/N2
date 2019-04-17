#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec2 vertexTexCoords_modelspace;

layout(location = 3) in mat4 model;
layout(location = 7) in vec2 vertexTexture_OffsetCurrent;
layout(location = 8) in vec2 vertexTexture_OffsetNext;
layout(location = 9) in float lifeTimeBlend;

uniform mat4 viewMatrix;
uniform mat4 projection;

out vec2 textureOffsetNext;
out vec2 textureOffsetCurrent;
out float textureBlend;

void main(){

	vec2 textureOriginal = vertexPosition_modelspace.xy + vec2(0.5, 0.5);
	textureOriginal /= 8;

	textureOffsetCurrent = textureOriginal + vertexTexture_OffsetCurrent;
	textureOffsetNext = textureOriginal + vertexTexture_OffsetNext;
	textureBlend = lifeTimeBlend;

	gl_Position = projection * viewMatrix * model * vec4(vertexPosition_modelspace, 1.0);
}

