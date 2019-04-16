#ifndef PARTICLECOMPONENT_H
#define PARTCILECOMPONENT_H

#include "Component.h"
#include "TransformComponent.h"
#include "Vector3.h"
#include "Entity.h"


//class Entity;

enum EmitterType {
	EMITTER_SPHERE,
	EMITTER_LINE,
	EMITTER_CONE,
	EMITTER_CYLINDER,
};


struct Particle {

	Particle(Vector3 position, Vector3 velocity, Vector3 colour) : position(position), velocity(velocity), colour(colour) {}
	Particle() : active(false), lifeTime(0.0f) {}


	bool operator < (const Particle& other) const
	{
		return this->cameraDist > other.cameraDist;
	}


	bool active;

	Vector3 position;
	Vector3 velocity;
	Vector3 colour;

	float cameraDist;

	float lifeTime;
};

class ParticleComponent : public Component
{
public:

	ParticleComponent(Entity* parent, EmitterType type, bool emitting, float lifeTime, float particlesPerSecond, Vector3 initialVelocity);
	ParticleComponent(Entity* parent);
	ParticleComponent();
	~ParticleComponent();


	void setSpawnTimer(const float& time);

	const bool& isEmitting() const;
	const float& getSpawnLifeTime() const;
	const float& getSpawnRate() const;
	const float& getSpawnTimer() const;
	const Vector3& getInitialVelocity() const;

	const Vector3& getPosition() const;
	const Vector3& getColour() const;
	const float& getLifeTime() const;

	const EmitterType& getType() const;

	std::vector<Particle*>& getParticles();

private:

	std::vector<Particle*> particles;

	EmitterType type;
	bool emitting;

	/* How many seconds for each particle */
	float particleSpawnRate;
	/* Timer for when a particle should spawn */
	float particleSpawnTimer;
	
	Vector3 position;
	Vector3 colour;
	float lifeTime;
	Vector3 initialVelocity;
};


#endif