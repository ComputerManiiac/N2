#version 330 core

out vec4 color;

uniform vec3 quadColour;
uniform float quadAlpha;


void main(){

	color = vec4(quadColour, quadAlpha);
}