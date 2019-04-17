#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "System.h"
#include "ParticleComponent.h"
#include "MatrixStack.h"
#include <map>
#include <vector>
#include <algorithm>

#define PARTICLE_MAX 20

struct ParticleData {

	ParticleData(Mtx44 model, Vector2 textureCurrent, Vector2 textureNext, float lifeTimeBlend) : 
		model(model), textureCurrent(textureCurrent), textureNext(textureNext), lifeTimeBlend(lifeTimeBlend) {}

	void setAll(const Mtx44& model, const Vector2& textureCurrent, const Vector2& textureNext, const float& lifeTimeBlend)
	{
		this->model = model;
		this->textureCurrent = textureCurrent;
		this->textureNext = textureNext;
		this->lifeTimeBlend = lifeTimeBlend;
	}

	Mtx44 model;
	Vector2 textureCurrent;
	Vector2 textureNext;
	float lifeTimeBlend;
};

class ParticleSystem : public System
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Initialize();
	void Update(double& dt);

	void registerComp(Component* component);
	void removeComp(Component* component);

	const std::vector<Particle*>& getParticlesFromEmitter(ParticleComponent* emitter);

private:

	Mtx44 removeRotationFromModel(const Mtx44& viewMatrix, const Mtx44& other);
	void initializeParticle(const ParticleComponent* emitter, Particle& particle);
	void updateTexture(const Particle& particle);

	void setTextureOffset(Vector2& textureOffset, const int& index);

	float randomFloat(float a, float b);
	float randomFloat(float a);
	Vector3 randomPointInSphere(const float& radius);
	Vector3 randomPointInCone(const Vector3& coneRotation, const float& coneWidthAngle);
	Vector3 randomPointInCircle(const float& radius);

	MS modelStack;
	unsigned int textureID;
	unsigned int emitterVAO;
	unsigned int emitterVBO;
	unsigned int emitterEBO;
	unsigned int emitterBatchVBO;


	int findFreeParticleIndex();
	int lastUsedParticle;
	unsigned int particleCount;
	Particle particleCollection[PARTICLE_MAX];
	std::map<ParticleComponent*, std::vector<ParticleData>> data;
	std::map<ParticleComponent*, std::vector<Particle*>> emitterCollection;
	std::vector<ParticleComponent*> subscribers;
};

#endif