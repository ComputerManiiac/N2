#version 330 core

/* Per Model Data */
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

/* Per Instance Data */
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in mat4 model;

//layout(location = 7) in mat4 viewMatrix;
//layout(location = 11) in mat4 projection;


/* Fragment Shader Output */
out vec3 vertexPosition_cameraspace;
out vec3 vertexNormal_cameraspace;
out vec2 texCoord;
out mat4 view;
out vec4 vertexPosition_lightspace;


uniform mat4 viewMatrix;
uniform mat4 projection;

/* Shadow Properties */
uniform mat4 lightProjectionView;
uniform bool lightEnabled;

/* Grass Properties */
uniform sampler2D windTexture;
uniform float timeElapsed;
uniform float windSpeed;
uniform float windStrength;
uniform vec3 objPosition;
const float radius = 1.0f;


void main(){

	mat4 MV = viewMatrix * model;
	mat4 MVP = projection * MV;
	vec4 finalPosition = MVP * vec4(vertexPosition_modelspace, 1);

	/* Wind only affects the grass near the top, when tex coord >= 0.8 */
	float moveFactor = step(0.8, vertexTexCoord.y);
	float factor = moveFactor * windStrength * vertexTexCoord.y;
	float period = moveFactor * (timeElapsed + gl_InstanceID * gl_VertexID + texture2D(windTexture, vertexTexCoord).a) * windSpeed;
	finalPosition.x += factor * sin(period);
	finalPosition.z += factor * cos(period) * 0.01f;


	/* Interaction with Objects */
	vec3 vertexWorld = (model * vec4(vertexPosition_modelspace, 1)).xyz;
	float dist = distance(vertexWorld, objPosition);
	float displaceScalar = 1 - clamp(dist / radius, 0, 1);
	vec3 sphereDisplace = normalize(vertexWorld - objPosition) * displaceScalar;
	finalPosition.x += sphereDisplace.x;
	finalPosition.z += sphereDisplace.z * 0.05f;


	gl_Position = finalPosition;

	// Vector position, in camera space
	vertexPosition_cameraspace = ( MV * vec4(vertexPosition_modelspace, 1) ).xyz;
	vertexPosition_lightspace = lightProjectionView * model * vec4(vertexPosition_modelspace, 1.0);
	
	view = viewMatrix;

	if(lightEnabled == true)
	{
		mat4 MV_inverse_transpose = transpose(inverse(MV));
		// Vertex normal, in camera space
		// Use MV if ModelMatrix does not scale the model ! Use its inverse transpose otherwise.
		vertexNormal_cameraspace = ( MV_inverse_transpose * vec4(vertexNormal_modelspace, 0) ).xyz;
	}
	// A simple pass through. The texCoord of each fragment will be interpolated from texCoord of each vertex
	texCoord = vertexTexCoord;
}

