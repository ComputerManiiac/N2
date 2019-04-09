#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Vector3.h"
#include "ShaderProgram.h"
#include "MatrixStack.h"
#include <vector>
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

	void setPosition(const Vector3& position);
	void setPosition(const float& x, const float& y, const float& z);
	void setPower(const float& power);
	void setColor(const Vector3& color);
	void setSpotlightDir(const Vector3& direction);

	const Vector3& getPosition() const;

	void setupAttribs();
	static unsigned int& getCount();
	static void setShaders(const std::vector<ShaderProgram*>& shaders);

private:

	const char* getPropertyName(const LIGHT_PROPERTIES& propertyName);
	
	static std::vector<ShaderProgram*> shaders;

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