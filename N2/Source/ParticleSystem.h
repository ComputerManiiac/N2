#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "System.h"
#include "ParticleComponent.h"
#include "MatrixStack.h"
#include "Info.h"
#include <map>
#include <vector>
#include <algorithm>

#define PARTICLE_MAX 10


class ParticleSystem : public System
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Initialize();
	void Update(double& dt);

	void registerComp(Component* component);
	void removeComp(Component* component);

	const std::vector<ParticleComponent*>& getSubscribers();

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
	std::vector<ParticleComponent*> subscribers;
};

#endif