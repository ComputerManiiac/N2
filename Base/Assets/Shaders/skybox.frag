#version 330 core
out vec4 color;

in vec2 texCoords;
in float fogVisibility;

uniform sampler2D skyboxTexture;
uniform sampler2D nightTexture;

const vec3 fogColor = vec3(0.5, 0.5, 0.5);
const float upperLimit = 50.0;
const float lowerLimit = 0.0;

uniform float time;

void main()
{    
//   finalColor = mix(vec4(fogColour, 1.0), finalColor, fogVisibility[index]);
//	float fogFactor = (texCoords.y - lowerLimit) / (upperLimit - lowerLimit);
//	fogFactor = clamp(fogFactor, 0.0, 1.0);
	
//	vec4 textureColor = texture2D(skyboxTexture, texCoords);
//	int timeInDay = time * 7200;
//	if(timeInDay > 86400)
//		timeInDay = time % 86400;
//
//	float t = timeInDay / 86400;
	vec4 textureColor = mix(texture2D(skyboxTexture, texCoords), texture2D(nightTexture, texCoords), time / 86400);
	color = mix(vec4(fogColor, 1.0), textureColor, fogVisibility);
}