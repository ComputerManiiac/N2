#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skyboxCubeMap;

void main()
{    
    FragColor = texture(skyboxCubeMap, texCoords);
}