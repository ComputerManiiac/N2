#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "Component.h"
#include "OBJInfo.h"
#include <vector>

class Entity;



class ColliderComponent : public Component
{
public:
	ColliderComponent(Entity* parent);
	ColliderComponent();
	~ColliderComponent();

	/* Initialized by RenderSystem*/
	void setBufferObjects(const unsigned int& VAO, const unsigned int& VBO, const unsigned int& EBO);
	const unsigned int& getVAO() const;
	const unsigned int& getVBO() const;
	const unsigned int& getEBO() const;
	const Vector3& getScale() const;
	const bool& shouldDrawCollider() const;

	std::vector<Vector3> getVertices() const;
	std::vector<Vector3> getVertices(const Vector3& translation, const Vector3& rotation) const;

	const OBJInfo& getInfo() const;

private:
	OBJInfo info;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	bool drawCollider;
	Vector3 scale;
	std::vector<Vector3> extremePoints;
};

#endif