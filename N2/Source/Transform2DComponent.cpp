#include "Transform2DComponent.h"


Transform2DComponent::Transform2DComponent(Entity* parent, Vector3 position, float rotation, Vector2 scale)
	: Component("Transform2D", parent), position(position), rotation(rotation), scale(scale), 
		scale3D(scale.x, scale.y, 1){

	centerPosition = position + scale3D * 0.5f;
}

Transform2DComponent::Transform2DComponent()
{
}


Transform2DComponent::~Transform2DComponent()
{
}

const Vector3& Transform2DComponent::getPos() const
{
	return position;
}

const Vector3& Transform2DComponent::getCenterPos() const
{
	return centerPosition;
}

const float& Transform2DComponent::getRot() const
{
	return rotation;
}


const Vector2& Transform2DComponent::getScale() const
{
	return scale;
}

const Vector3& Transform2DComponent::getScale3D() const
{
	return scale3D;
}

