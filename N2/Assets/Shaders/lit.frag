#version 330 core



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


// Values that stay constant for the whole mesh.
uniform bool lightEnabled;
uniform Light lights[MAX_LIGHTS];
uniform Material material;
uniform int numLights;
uniform bool colorTextureEnabled;
uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
//uniform mat4 view;

// Interpolated values from the vertex shaders
in vec3 vertexPosition_cameraspace;
in vec3 vertexNormal_cameraspace;
in vec2 texCoord;
in mat4 view;
in vec4 vertexPosition_lightspace;

// Ouput data
out vec4 color;


float getAttenuation(Light light, float distance) {
	if(light.type == 1)
		return 1;
	else
		return 1 / max(1, light.kC + light.kL * distance + light.kQ * distance * distance);
}

float getSpotlightEffect(Light light, vec3 lightDirection) {
	vec3 spotDir = -(view * vec4(light.spotDirection, 0.0)).xyz;
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
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0 )
		return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthTexture, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(depthTexture, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(depthTexture, projCoords.xy + vec2(x, y) * texelSize).r; 
			if(currentDepth > pcfDepth)
				shadow+=1.0;
		}    
	}
	shadow /= 9.0;
	

    return shadow;
}


void main(){
	if(lightEnabled == true)
	{

		// Material properties
		vec4 materialColor;
		if(colorTextureEnabled == true)
			materialColor = texture2D( colorTexture, texCoord );
		else
			materialColor = vec4( 0, 1, 0, 1 );

		// Vectors
		vec3 eyeDirection_cameraspace = -vertexPosition_cameraspace;
		vec3 E = normalize(eyeDirection_cameraspace);
		vec3 N = normalize( vertexNormal_cameraspace );
		
		color = vec4(0, 0, 0, 0);
		
		for(int i = 0; i < numLights; i++)	
		{
			// Light direction
			float spotlightEffect = 1;
			vec3 lightDirection_cameraspace;
			vec3 lightPosition_cameraspace = (view * vec4(lights[i].position, 1.0)).xyz;
			if(lights[i].type == 1) {
				lightDirection_cameraspace = lightPosition_cameraspace;
			}
			else if(lights[i].type == 2) {
				lightDirection_cameraspace = lightPosition_cameraspace - vertexPosition_cameraspace;
				spotlightEffect = getSpotlightEffect(lights[i], lightDirection_cameraspace);
			}
			else {
				lightDirection_cameraspace = lightPosition_cameraspace - vertexPosition_cameraspace;
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
			
			 color += ambient + (1.0 - getShadow(vertexPosition_lightspace, L, N)) * (diffuse + specular);
//			color = vec4(vec3(getShadow(vertexPosition_lightspace)), 1.f);
//			color += ambient + diffuse + specular;
		}
	}
	else
	{
		if(colorTextureEnabled == true)
			color = texture2D( colorTexture, texCoord );
		else
			color = vec4( 0, 1, 0, 1 );
	}
}