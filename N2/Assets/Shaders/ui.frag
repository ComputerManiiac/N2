#version 330 core

in vec3 textColour;
out vec4 color;

void main(){
	color = vec4(textColour, 1.0);
}