#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;


struct Light {
	int type;
	vec3 position;
	vec3 color;
	float power;
	float kC;
	float kL;
	float kQ;
	vec3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;
};

struct Material {
	vec3 kAmbient;
	vec3 kDiffuse;
	vec3 kSpecular;
	float kShininess;
};

// Constant values
const int MAX_LIGHTS = 8;

in vec3 vertexPosition_worldspace[];
in vec3 vertexPosition_cameraspace[];
in vec3 vertexNormal_cameraspace[];
in vec2 texCoord[];
in mat4 view[];
in vec4 vertexPosition_lightspace[];

uniform bool lightEnabled;
uniform Light lights[MAX_LIGHTS];
uniform Material material;
uniform int numLights;
uniform bool colorTextureEnabled;
uniform sampler2D colorTexture;
uniform sampler2D depthTexture;

out vec4 finalColor;
out int shouldDiscard;


float getAttenuation(Light light, float distance) {
	if(light.type == 1)
		return 1;
	else
		return 1 / max(1, light.kC + light.kL * distance + light.kQ * distance * distance);
}

float getSpotlightEffect(Light light, vec3 lightDirection, int index) {
	vec3 spotDir = -(view[index] * vec4(light.spotDirection, 0.0)).xyz;
	vec3 S = normalize(spotDir);
	vec3 L = normalize(lightDirection);
	float cosDirection = dot(L, S);
	//return smoothstep(light.cosCutoff, light.cosInner, cosDirection);
	if(cosDirection < light.cosCutoff)
		return 0;
	else
		return 1; //pow(cosDirection, light.exponent);
}

float getShadow(vec4 lightSpacePos, vec3 lightDir, vec3 vertexNormal){

	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0 )
		return 0.0;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthTexture, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;


	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(depthTexture, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(depthTexture, projCoords.xy + vec2(x, y) * texelSize).r; 
//			if(currentDepth - 0.0001f > pcfDepth)
			if(currentDepth - 0.0004f > pcfDepth)
				shadow+=1.0;
		}    
	}
	shadow /= 9.0;
	
    return shadow;
} 

void calculateLighting(vec3 normal, int index){
	if(lightEnabled == true)
	{

		// Material properties
		vec4 materialColor;
		if(colorTextureEnabled == true)
			materialColor = texture2D( colorTexture, texCoord[index]);
		else
			materialColor = vec4( 0, 1, 0, 1 );


		// Vectors
		vec3 eyeDirection_cameraspace = -vertexPosition_cameraspace[index];
		vec3 E = normalize(eyeDirection_cameraspace);
		vec3 N = normalize(normal);
		
		finalColor = vec4(0, 0, 0, 0);
		
		for(int i = 0; i < numLights; i++)	
		{
			// Light direction
			float spotlightEffect = 1;
			vec3 lightDirection_cameraspace;
			vec3 lightPosition_cameraspace = (view[index] * vec4(lights[i].position, 1.0)).xyz;
			if(lights[i].type == 1) {
				lightDirection_cameraspace = lightPosition_cameraspace;
			}
			else if(lights[i].type == 2) {
				lightDirection_cameraspace = lightPosition_cameraspace - vertexPosition_cameraspace[0];
				spotlightEffect = getSpotlightEffect(lights[i], lightDirection_cameraspace, index);
			}
			else {
				lightDirection_cameraspace = lightPosition_cameraspace - vertexPosition_cameraspace[0];
			}
			// Distance to the light
			float distance = length( lightDirection_cameraspace );
			
			// Light attenuation
			float attenuationFactor = getAttenuation(lights[i], distance);

			vec3 L = normalize( lightDirection_cameraspace );
			float cosTheta = clamp( dot( N, L ), 0, 1 );
			
			vec3 R = reflect(-L, N);
			float cosAlpha = clamp( dot( E, R ), 0, 1 );
			
			vec4 ambient = materialColor * vec4(material.kAmbient, 1);
			vec4 diffuse = materialColor * vec4(material.kDiffuse, 1) * vec4(lights[i].color, 1) * lights[i].power * cosTheta * attenuationFactor * spotlightEffect;
			vec4 specular = vec4(material.kSpecular, 1) * vec4(lights[i].color, 1) * lights[i].power * pow(cosAlpha, material.kShininess) * attenuationFactor * spotlightEffect;
			
			finalColor += ambient + (1.0 - getShadow(vertexPosition_lightspace[index], L, N)) * (diffuse + specular);

		}

		if(materialColor.a < 0.1)
			shouldDiscard = 1;
		else
			shouldDiscard = 0;

		finalColor.a =  materialColor.a;

	}
	else
	{
		if(colorTextureEnabled == true)
			finalColor = texture2D( colorTexture, texCoord[index]);
		else
			finalColor = vec4( 0, 1, 0, 1 );
	}
}

vec3 calculateTriangleNormalCameraSpace(){
	vec3 tangent = vertexPosition_worldspace[1] - vertexPosition_worldspace[0];
	vec3 bitangent = vertexPosition_worldspace[2] - vertexPosition_worldspace[0];
	vec3 normal = cross(tangent, bitangent);	
	return (view[0] * vec4(normalize(normal), 0.0)).xyz;
}

void setVertex(vec3 normal, int index){

	gl_Position = gl_in[index].gl_Position;
	calculateLighting(normal, index);
	EmitVertex();
}


void main(){
	
	vec3 normal = calculateTriangleNormalCameraSpace();
//	vec3 normal = vec3(0,0,0);
	setVertex(normal, 0);
	setVertex(normal, 1);
	setVertex(normal, 2);
	EndPrimitive();

}

