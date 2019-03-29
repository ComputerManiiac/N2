#include "TransformComponent.h"

TransformComponent::TransformComponent(Entity* parent, Vector3 pos, Vector3 rot, Vector3 scale) : Component("Transform", parent)
{
	this->position = pos;
	setRot(rot);
	this->scale = scale;
}


TransformComponent::TransformComponent(Entity* parent) : Component("Transform", parent)
{
	position.SetZero();
	setRot(Vector3(0, 0, 0));
	scale.Set(1, 1, 1);
}

TransformComponent::TransformComponent()
{
}


TransformComponent::~TransformComponent()
{
}

void TransformComponent::Move(const Vector3& delta)
{
	position += delta;
}

void TransformComponent::Rotate(const Vector3& delta)
{
	rotation += delta;
}

void TransformComponent::setPos(const Vector3& pos)
{
	this->position = pos;
}

void TransformComponent::setRot(const Vector3& rot)
{
	this->rotation = rot;
	
	// X Axis
	localAxes[0].Set(1, 0, 0);
	localAxes[0].Rotate(rotation.x, rotation.y, 0);

	localAxes[1].Set(0, 1, 0);
	localAxes[1].Rotate(rotation.x, 0, rotation.z);

	localAxes[2].Set(0, 0, 1);
	localAxes[2].Rotate(rotation.x, rotation.y, 0);

	//// Y Axis
	//localAxes[1] = Vector3(0, 1, 0).Rotate(Vector3(rotation.x, 0, 0)).Rotate(Vector3(0, 0, rotation.z));
	//// Z Axis
	//localAxes[2] = Vector3(0, 0, 1).Rotate(Vector3(0, rotation.y, 0)).Rotate(Vector3(rotation.x, 0, 0));


	//// X Axis
	//localAxes[0] = Vector3(1, 0, 0).Rotate(Vector3(0, rotation.y, 0)).Rotate(Vector3(0, 0, rotation.z));
	//// Y Axis
	//localAxes[1] = Vector3(0, 1, 0).Rotate(Vector3(rotation.x, 0, 0)).Rotate(Vector3(0, 0, rotation.z));
	//// Z Axis
	//localAxes[2] = Vector3(0, 0, 1).Rotate(Vector3(0, rotation.y, 0)).Rotate(Vector3(rotation.x, 0, 0));
}

void TransformComponent::setScale(const Vector3 & scale)
{
	this->scale = scale;
}

const Vector3& TransformComponent::getPos() const
{
	return position;
}

const Vector3& TransformComponent::getRot() const
{
	return rotation;
}	

const Vector3& TransformComponent::getScale() const
{
	return scale;
}

Vector3* TransformComponent::getLocalAxes()
{
	return &localAxes[0];
}



