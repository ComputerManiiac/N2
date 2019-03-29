#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class Entity;

class Component
{
public:

	Component(std::string name, Entity* parent, bool enabled = true);
	Component();
	virtual ~Component();

	void setName(const std::string& name);
	const std::string& getName() const;

	void setParent(Entity* parent);
	Entity* getParent();

	void setActive(const bool& active);
	bool isActive();

protected:

	bool enabled;
	std::string name;
	Entity* parent;
};

#endif