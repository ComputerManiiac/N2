#version 330 core



// Values that stay constant for the whole mesh.

//uniform mat4 view;

// Interpolated values from the geometry shaders
in vec4 finalColor;
in int shouldDiscard;

// Ouput data
out vec4 color;


void main(){


	color = finalColor;
}