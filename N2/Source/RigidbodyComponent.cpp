#include "RigidbodyComponent.h"

unsigned int RigidbodyComponent::idCount = 0;

RigidbodyComponent::RigidbodyComponent(Entity* parent, float mass) : Component("Rigidbody", parent), mass(mass), inverseMass(1.0f/mass), id(idCount++)
{

}

RigidbodyComponent::RigidbodyComponent()
{
}


RigidbodyComponent::~RigidbodyComponent()
{
}

void RigidbodyComponent::Calculate()
{
	velocity = momentum * inverseMass;
}



void RigidbodyComponent::addForce(const Vector3& force)
{
	netForce += force;
}

void RigidbodyComponent::addTorque(const Vector3& torque)
{
	netTorque += torque;
}

void RigidbodyComponent::setValues(const Vector3& momentum, const Vector3& velocity)
{
	this->momentum = momentum;
	this->velocity = velocity;
}

void RigidbodyComponent::setVelocity(const Vector3& velocity)
{
	this->velocity = velocity;
}

void RigidbodyComponent::resetUniformermediateValues()
{
	netForce = netTorque = Vector3(0, 0, 0);

}

const unsigned int& RigidbodyComponent::getID() const
{
	return id;
}

const Vector3& RigidbodyComponent::getNetForce() const
{
	return netForce;
}

const Vector3 & RigidbodyComponent::getVelocity() const
{
	return velocity;
}

const Vector3 & RigidbodyComponent::getMomentum() const
{
	return momentum;
}

const float & RigidbodyComponent::getMass() const
{
	return mass;
}

const float& RigidbodyComponent::getInverseMass() const
{
	return inverseMass;
}

const Vector3& RigidbodyComponent::getNetTorque() const
{
	return netTorque;
}

const Vector3& RigidbodyComponent::getAngularVelocity() const
{
	return angularVelocity;
}
