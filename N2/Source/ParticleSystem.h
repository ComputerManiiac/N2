#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "System.h"
#include "ParticleComponent.h"
#include <map>
#include <vector>
#include <algorithm>

#define PARTICLE_MAX 1000


class ParticleSystem : public System
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Initialize();
	void Update(double& dt);

	void registerComp(Component* component);
	void removeComp(Component* component);

private:

	int findFreeParticleIndex();
	int lastUsedParticle;
	unsigned int particleCount;
	Particle particleCollection[PARTICLE_MAX];
	std::map<ParticleComponent*, std::vector<Particle*>> emitterCollection;
	std::vector<ParticleComponent*> subscribers;
};

#endif