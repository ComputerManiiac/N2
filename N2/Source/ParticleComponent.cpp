#include "ParticleComponent.h"
#include "Entity.h"


ParticleComponent::ParticleComponent(Entity* parent, EmitterType type, bool emitting, float lifeTime, float particlesPerSecond, Vector3 initialVelocity)
	: Component("Particle Emitter", parent), type(type), emitting(emitting), lifeTime(lifeTime), initialVelocity(initialVelocity), particleSpawnTimer(0)
{
	particleSpawnRate = 1.0f / particlesPerSecond;
	position = getParent()->getComponent<TransformComponent>()->getPos();
	colour.Set(1, 0, 1);
}


ParticleComponent::ParticleComponent(Entity* parent) : Component("Particle Emitter", parent)
{

}

ParticleComponent::ParticleComponent()
{
}


ParticleComponent::~ParticleComponent()
{
}

void ParticleComponent::setSpawnTimer(const float& time)
{
	particleSpawnTimer = time;
}


const bool& ParticleComponent::isEmitting() const
{
	return emitting;
}

const float& ParticleComponent::getSpawnLifeTime() const
{
	return lifeTime;
}

const float& ParticleComponent::getSpawnRate() const
{
	return particleSpawnRate;
}

const float& ParticleComponent::getSpawnTimer() const
{
	return particleSpawnTimer;
}

const Vector3& ParticleComponent::getInitialVelocity() const
{
	return initialVelocity;
}


const Vector3& ParticleComponent::getPosition() const
{
	return position;
}

const Vector3& ParticleComponent::getColour() const
{
	return colour;
}

const float& ParticleComponent::getLifeTime() const
{
	return lifeTime;
}

const EmitterType & ParticleComponent::getType() const
{
	return type;
}

std::vector<Particle*>& ParticleComponent::getParticles()
{
	return particles;
}

