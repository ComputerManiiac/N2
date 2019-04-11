#include "ParticleSystem.h"



ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Initialize()
{
	particleCount = 0;
	lastUsedParticle = -1;
}

void ParticleSystem::Update(double& dt)
{
	for (ParticleComponent* emitter : subscribers)
	{
		std::vector<Particle*>& particles = emitterCollection[emitter];
		const float& spawnTimer = emitter->getSpawnTimer() - dt;


		/* Spawn Particle */
		if (particleCount < PARTICLE_MAX && spawnTimer <= 0.0f)
		{

			int index = findFreeParticleIndex();
			if (index == -1)
				std::cout << "[Error] No free particles although particle count is not maxed?" << std::endl;

			Particle& newParticle = particleCollection[index];
			newParticle.setDefault(emitter);
			particles.push_back(&newParticle);
			

			emitter->setSpawnTimer(emitter->getSpawnRate());
			std::cout << "Current Particle Count: " << particleCount << std::endl;
		}
		else
		{
			emitter->setSpawnTimer(spawnTimer);
		}



		/* Update Particles */
		for(int i = 0; i < particles.size(); i++)
		{
			Particle* particle = particles[i];
			particle->position += particle->velocity * dt;
			particle->lifeTime -= dt;

			/* Particle has expired */
			if (particle->lifeTime <= 0.0f)
			{
				std::cout << "I'm dead!" << std::endl;
				particle->active = false;
				particles.erase(particles.begin() + i);
				--particleCount;
			}

			
		}

	}
}


int ParticleSystem::findFreeParticleIndex()
{
	std::cout << "Last Used Particle: " << lastUsedParticle << std::endl;

	/* Find an inactive particle from the position of the last used particle to the end of the container */
	for (int i = lastUsedParticle + 1; i < PARTICLE_MAX; i++)
	{
		if (!particleCollection[i].active)
		{
			lastUsedParticle = i;
			++particleCount;
			return i;
		}
	}

	/* Find an inactive particle from the start*/
	for (int i = 0; i < lastUsedParticle; i++)
	{
		if (!particleCollection[i].active)
		{
			lastUsedParticle = i;
			++particleCount;
			return i;
		}
	}

	/* Return -1 if there is no free particle*/
	return -1;
}


void ParticleSystem::registerComp(Component* component)
{
	ParticleComponent* particle = static_cast<ParticleComponent*>(component);
	if (particle == nullptr) return;
	subscribers.push_back(particle);
}

void ParticleSystem::removeComp(Component* component)
{
	ParticleComponent* particle = static_cast<ParticleComponent*>(component);
	if (particle == nullptr) return;
	subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), particle));
}
