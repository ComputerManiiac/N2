#ifndef RIGIDBODY_COMPONENT_H
#define RIGIDBODY_COMPONENT_H

#include "Component.h"
#include "Vector3.h"

class RigidbodyComponent : public Component
{
public:

	RigidbodyComponent(Entity* parent, float mass);
	RigidbodyComponent();
	~RigidbodyComponent();

	void Calculate();

	void addForce(const Vector3& force);
	void addTorque(const Vector3& torque);
	void setValues(const Vector3& momentum, const Vector3& velocity);
	void setVelocity(const Vector3& velocity);
	void resetUniformermediateValues();

	const unsigned int& getID() const;

	const Vector3& getNetForce() const;
	const Vector3& getVelocity() const;
	const Vector3& getMomentum() const;
	const float& getMass() const;
	const float& getInverseMass() const;
	const Vector3& getNetTorque() const;
	const Vector3& getAngularVelocity() const;

private:

	static unsigned int idCount;
	unsigned int id;
	// Translational
	Vector3 netForce;
	Vector3 momentum;
	Vector3 velocity;

	// Rotational
	Vector3 netTorque;
	Vector3 angularVelocity;

	// Constants
	float mass;
	float inverseMass;
};

#endif 