#include "Transform2DComponent.h"


Transform2DComponent::Transform2DComponent(Entity* parent, Vector3 position, Vector2 rotation, Vector2 scale)
	: Component("Transform2D", parent), position(position), rotation(rotation), scale(scale){

}

Transform2DComponent::Transform2DComponent()
{
}


Transform2DComponent::~Transform2DComponent()
{
}
