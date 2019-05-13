#version 330 core



// Values that stay constant for the whole mesh.

//uniform mat4 view;

// Interpolated values from the geometry shaders
in vec4 finalColor;
in float materialAlpha;

// Ouput data
out vec4 color;


void main(){

	if(materialAlpha < 0.1)
		discard;
	color = finalColor;
}