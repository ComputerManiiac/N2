#ifndef BATCHDATA_H
#define BATCHDATA_H

#include "Mtx44.h"
#include "Info.h"
#include <vector>

struct BatchData {
	BatchData(Mtx44 model) : model(model) {}
	BatchData(){}
	Mtx44 model;
};

struct ParticleData : public BatchData {

	ParticleData(Mtx44 model, Vector2 textureCurrent, Vector2 textureNext, float lifeTimeBlend) :
		BatchData(model), textureCurrent(textureCurrent), textureNext(textureNext), lifeTimeBlend(lifeTimeBlend) {}

	void setAll(const Mtx44& model, const Vector2& textureCurrent, const Vector2& textureNext, const float& lifeTimeBlend)
	{
		this->model = model;
		this->textureCurrent = textureCurrent;
		this->textureNext = textureNext;
		this->lifeTimeBlend = lifeTimeBlend;
	}

	Vector2 textureCurrent;
	Vector2 textureNext;
	float lifeTimeBlend;
};

#endif