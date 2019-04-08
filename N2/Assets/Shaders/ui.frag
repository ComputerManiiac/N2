#version 330 core

in vec2 texCoords;
out vec4 color;

uniform vec3 textColor;
uniform float textSize;
uniform sampler2D fontTexture;

const vec3 shadowColor = vec3(0,0,0);
const vec3 outlineColor = vec3(1,1,1);
/* Between 0 and spread / textureSize */
const vec2 shadowOffset = vec2(0.004, -0.004);
/* Between 0 and 0.5 */
const float shadowSmoothing = 0.2;

void main(){

	float smoothing = 0.50f / (4.0f * textSize);
	float outlineDistance = 0.0f;
	//float outlineDistance = 0.4 - (textSize * 0.05);

    float dist = texture2D(fontTexture, texCoords).a;
    float outlineFactor = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist);

    vec3 finalColor = mix(outlineColor, textColor, outlineFactor);
    float alpha = smoothstep(0.5 - outlineDistance - smoothing, 0.5 - outlineDistance + smoothing, dist);

	float shadowDistance = texture2D(fontTexture, texCoords - shadowOffset).a;
    float shadowAlpha = smoothstep(0.5 - shadowSmoothing, 0.5 + shadowSmoothing, shadowDistance);
    vec4 shadow = vec4(shadowColor.rgb, shadowAlpha);

	color = mix(shadow, vec4(finalColor, 1.0), alpha);
}