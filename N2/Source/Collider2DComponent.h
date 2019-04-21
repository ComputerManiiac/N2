#ifndef COLLIDER2D_COMPONENT_H
#define COLLIDER2D_COMPONENT_H

#include "Component.h"

class Collider2DComponent : public Component
{
public:
	Collider2DComponent(Entity* parent);
	Collider2DComponent();
	~Collider2DComponent();
};

#endif