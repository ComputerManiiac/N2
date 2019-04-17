#ifndef TRANSFORM2DCOMPONENT_H
#define TRANSFORM2DCOMPONENT_H

#include "Component.h"
#include "Info.h"

class Transform2DComponent : public Component
{
public:
	
	Transform2DComponent(Entity* parent, Vector3 position, Vector2 rotation, Vector2 scale);
	Transform2DComponent();
	~Transform2DComponent();

private:
	Vector3 position;
	Vector2 rotation;
	Vector2 scale;
};

#endif