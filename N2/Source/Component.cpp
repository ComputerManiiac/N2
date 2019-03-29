#include "Component.h"
#include "Entity.h"

Component::Component(std::string name, Entity* parent, bool enabled)
{
	this->name = name;
	this->parent = parent;
	this->enabled = enabled;
}


Component::Component()
{
	this->name = "";
	this->parent = nullptr;
	this->enabled = true;
}


Component::~Component()
{
}


void Component::setName(const std::string& name)
{
	this->name = name;
}

const std::string& Component::getName() const
{
	return name;
}

void Component::setParent(Entity* parent)
{
	this->parent = parent;
}

Entity* Component::getParent()
{
	return parent;
}

void Component::setActive(const bool & active)
{
	enabled = active;
}

bool Component::isActive()
{
	return enabled;
}
