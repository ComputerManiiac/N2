#version 330 core

in vec2 textureOffsetCurrent;
in vec2 textureOffsetNext;
in float textureBlend;
out vec4 color;

uniform sampler2D particleTexture;

void main(){

//	color = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 currentColour = texture2D(particleTexture, textureOffsetCurrent);
	vec4 nextColour = texture2D(particleTexture, textureOffsetNext);
	color = mix(currentColour, nextColour, textureBlend);
}