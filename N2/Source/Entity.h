#ifndef ENTITY_H
#define ENTITY_H

#include "Contiguous.h"
#include "OverloadNew.h"
#include "Info.h"


#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"

#include <map>
#include <typeinfo>
#include <typeindex>


class Entity
{
public:

	Entity(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader, std::string texturePath);
	Entity(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader, std::string modelPath, std::string texturePath);

	Entity(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader, bool collisionEnabled, std::string modelPath, std::string texturePath);

	Entity(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader);
	Entity();
	virtual ~Entity();


	const std::string& getName() const;


	template<typename T>
	void addComponent(T* component);

	template<typename T>
	T* getComponent();

private:

	Contiguous contiguous;
	std::string name;
	std::map<std::type_index, Component*> components;
};


template<typename T>
void Entity::addComponent(T* component)
{
	std::type_index id = std::type_index(typeid(component));
	components[id] = component;
}

template<typename T>
T* Entity::getComponent()
{

	std::type_index id = std::type_index(typeid(T*));
	if (components.find(id) != components.end())
	{
		Component* comp = components[id];
		T* tComp = static_cast<T*>(comp);
		return tComp;
	}
	else
	{
		return nullptr;
	}
}

#endif