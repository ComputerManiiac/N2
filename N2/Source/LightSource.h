#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Vector3.h"
#include "ShaderProgram.h"
#include "MatrixStack.h"
#include <iostream>
#include <string>
#include <map>


enum LIGHT_PROPERTIES {
	LIGHT_TYPE = 0,
	LIGHT_POSITION,
	LIGHT_COLOR,
	LIGHT_POWER,
	LIGHT_KC,
	LIGHT_KL,
	LIGHT_KQ,
	LIGHT_COSCUTOFF,
	LIGHT_COSINNER,
	LIGHT_EXPONENT,
	LIGHT_SPOTDIRECTION,
	LIGHT_ENABLED,
	LIGHT_COUNT,
	LIGHT_PROPERTIES_COUNT,
};

enum LIGHTING_TYPE {
	LIGHT_POINT = 0,
	LIGHT_DIRECTIONAL = 1,
	LIGHT_SPOTLIGHT = 2,

};


class LightSource
{
public:

	LightSource(LIGHTING_TYPE type);
	LightSource();
	~LightSource();

	void setPointLight(ShaderProgram* shader, const Vector3& position, const Vector3& color, const float& power,
		const float& kC, const float& kL, const float& kQ);
	
	void setDirLight(ShaderProgram* shader, const Vector3& direction, const Vector3& color, const float& power);
	void setSpotLight(ShaderProgram* shader, const Vector3& position, const Vector3& color, const Vector3& spotDirection,
		const float& power, const float& cosCutoff, const float& cosInner, const float& exponent,
		const float& kC, const float &kL, const float& kQ);

	inline void setPosition(const Vector3& position);
	inline void setPosition(const float& x, const float& y, const float& z);
	inline void setPower(const float& power);
	inline void setColor(const Vector3& color);
	inline void setSpotlightDir(const Vector3& direction);

	const Vector3& getPosition() const;

	void setupAttribs();

	static unsigned int& getCount();
	static void setShader(ShaderProgram* shader);

private:

	const char* getPropertyName(const LIGHT_PROPERTIES& propertyName);
	
	static ShaderProgram* shader;
	unsigned int id;
	std::string prefix;
	static unsigned int count;
	std::map<LIGHT_PROPERTIES, std::string> locationNames;

	LIGHTING_TYPE type;
	Vector3 position;
	Vector3 color;
	float power;
	float kC;
	float kL;
	float kQ;

	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;
};

#endif