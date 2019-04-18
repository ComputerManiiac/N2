#ifndef ENTITY2D_H
#define ENTITY2D_H

#include "Entity.h"

class Entity2D : public Entity
{
public:
	Entity2D(std::string name, Vector3 position, float rotation, Vector2 scale, ShaderProgram* shader, std::string texturePath, Vector2 textureOffset);
	Entity2D();
	~Entity2D();
};

#endif