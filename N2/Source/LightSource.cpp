#include "LightSource.h"


unsigned int LightSource::count = 0;

std::vector<ShaderProgram*> LightSource::shaders = {};

LightSource::LightSource(LIGHTING_TYPE type) :  type(type)
{
	id = count++;
	prefix = "lights[" + std::to_string(id) + "].";
}

LightSource::LightSource() {}


LightSource::~LightSource()
{
	count--;
}


void LightSource::setPointLight(const Vector3& position, const Vector3& color, const float& power,
	const float& kC, const float& kL, const float& kQ)
{
	this->position = position;
	this->color = color;
	this->power = power;
	this->kC = kC;
	this->kL = kL;
	this->kQ = kQ;

	locationNames.try_emplace(LIGHT_KC, prefix + "kC");
	locationNames.try_emplace(LIGHT_KL, prefix + "kL");
	locationNames.try_emplace(LIGHT_KQ, prefix + "kQ");
	locationNames.try_emplace(LIGHT_TYPE, prefix + "type");
	locationNames.try_emplace(LIGHT_POSITION, prefix + "position");
	locationNames.try_emplace(LIGHT_COLOR, prefix + "color");
	locationNames.try_emplace(LIGHT_POWER, prefix + "power");
	setupAttribs();
}

void LightSource::setDirLight(const Vector3& direction, const Vector3& color, const float& power)
{
	this->position = direction;
	this->color = color;
	this->power = power;
	locationNames.try_emplace(LIGHT_TYPE, prefix + "type");
	locationNames.try_emplace(LIGHT_POSITION, prefix + "position");
	locationNames.try_emplace(LIGHT_COLOR, prefix + "color");
	locationNames.try_emplace(LIGHT_POWER, prefix + "power");
	setupAttribs();
}

void LightSource::setSpotLight(const Vector3& position, const Vector3& color, const Vector3& spotDirection,
	const float& power, const float& cosCutoff, const float& cosInner, const float& exponent, 
	const float& kC, const float& kL, const float& kQ)
{
	this->position = position;
	this->color = color;
	this->power = power;
	this->spotDirection = spotDirection;
	this->cosCutoff = cosCutoff;
	this->cosInner = cosInner;
	this->exponent = exponent;
	this->kC = kC;
	this->kL = kL;
	this->kQ = kQ;

	locationNames.try_emplace(LIGHT_SPOTDIRECTION, prefix + "spotDirection");
	locationNames.try_emplace(LIGHT_COSCUTOFF, prefix + "cosCutoff");
	locationNames.try_emplace(LIGHT_COSINNER, prefix + "cosInner");
	locationNames.try_emplace(LIGHT_EXPONENT, prefix + "exponent");
	locationNames.try_emplace(LIGHT_KC, prefix + "kC");
	locationNames.try_emplace(LIGHT_KL, prefix + "kL");
	locationNames.try_emplace(LIGHT_KQ, prefix + "kQ");
	locationNames.try_emplace(LIGHT_TYPE, prefix + "type");
	locationNames.try_emplace(LIGHT_POSITION, prefix + "position");
	locationNames.try_emplace(LIGHT_COLOR, prefix + "color");
	locationNames.try_emplace(LIGHT_POWER, prefix + "power");
	setupAttribs();
}

void LightSource::setupAttribs()
{
	for (ShaderProgram* shader : shaders)
	{
		shader->Use();
		shader->setUniform(getPropertyName(LIGHT_TYPE), type);
		shader->setUniform("lightEnabled", true);

		if (type == LIGHT_POINT || type == LIGHT_SPOTLIGHT)
		{
			shader->setUniform(getPropertyName(LIGHT_KC), kC);
			shader->setUniform(getPropertyName(LIGHT_KL), kL);
			shader->setUniform(getPropertyName(LIGHT_KQ), kQ);

			if (type == LIGHT_SPOTLIGHT)
			{
				shader->setUniform(getPropertyName(LIGHT_COSCUTOFF), cos(Math::DegreeToRadian(cosCutoff)));
				shader->setUniform(getPropertyName(LIGHT_COSINNER), cos(Math::DegreeToRadian(cosInner)));
				shader->setUniform(getPropertyName(LIGHT_EXPONENT), exponent);
			}
		}

		shader->setUniform(getPropertyName(LIGHT_POSITION), position);
		shader->setUniform(getPropertyName(LIGHT_COLOR), color);
		shader->setUniform(getPropertyName(LIGHT_POWER), power);

		if (type == LIGHT_SPOTLIGHT)
			shader->setUniform(getPropertyName(LIGHT_SPOTDIRECTION), spotDirection);

		shader->setUniform("numLights", (int)LightSource::getCount());
	}
}


const char* LightSource::getPropertyName(const LIGHT_PROPERTIES& propertyName)
{
	return locationNames[propertyName].c_str();
}

unsigned int& LightSource::getCount()
{
	return count;
}

void LightSource::setShaders(const std::vector<ShaderProgram*>& shaders)
{
	LightSource::shaders = shaders;
}

void LightSource::setPosition(const Vector3& position)
{
	for (ShaderProgram* shader : shaders)
	{
		shader->Use();
		shader->setUniform(getPropertyName(LIGHT_POSITION), position);
	}
	this->position = position;
}


void LightSource::setPosition(const float& x, const float& y, const float& z)
{
	for (ShaderProgram* shader : shaders)
	{
		shader->Use();
		shader->setUniform(getPropertyName(LIGHT_POSITION), x, y, z);
	}
	this->position = position;
}

void LightSource::setColor(const Vector3& color)
{

	for (ShaderProgram* shader : shaders)
	{
		shader->Use();
		shader->setUniform(getPropertyName(LIGHT_COLOR), color);
	}
	this->color = color;
}

void LightSource::setSpotlightDir(const Vector3& spotDirection)
{
	for (ShaderProgram* shader : shaders)
	{
		shader->Use();
		shader->setUniform(getPropertyName(LIGHT_SPOTDIRECTION), spotDirection);
	}
	this->spotDirection = spotDirection;
}

const Vector3& LightSource::getPosition() const
{
	return position;
}

void LightSource::setPower(const float& power)
{
	for (ShaderProgram* shader : shaders)
	{
		shader->Use();
		shader->setUniform(getPropertyName(LIGHT_POWER), power);
	}
	this->power = power;
}



