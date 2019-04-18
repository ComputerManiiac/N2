#ifndef TRANSFORM2DCOMPONENT_H
#define TRANSFORM2DCOMPONENT_H

#include "Component.h"
#include "Info.h"

class Transform2DComponent : public Component
{
public:
	
	Transform2DComponent(Entity* parent, Vector3 position, float rotation, Vector2 scale);
	Transform2DComponent();
	~Transform2DComponent();

	void setPosition(const Vector3& position);

	const Vector3& getPos() const;
	const Vector3& getCenterPos() const;

	const float& getRot() const;

	
	const Vector2& getScale() const;
	const Vector3& getScale3D() const;

private:



	Vector3 position;
	Vector3 centerPosition;

	float rotation;
	Vector2 scale;
	Vector3 scale3D;
};

#endif