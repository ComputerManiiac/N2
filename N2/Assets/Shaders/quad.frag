#version 330 core

in vec2 texCoords;
out vec4 color;

float LinearizeDepth(float depth)
{
    float near_plane = 0.1f;
	float far_plane = 25.0f;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}


uniform sampler2D sceneTexture;

void main()
{             
    float depthValue = texture(sceneTexture, texCoords).r;
    color = vec4(vec3(LinearizeDepth(depthValue) / 25.0f), 1.0);
}