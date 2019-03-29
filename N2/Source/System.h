#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include "Component.h"

class System
{
public:
	System() {}
	virtual ~System() {}

	virtual void Initialize() = 0;
	virtual void Update(double& dt) = 0;
	virtual void registerComp(Component* component) = 0;
	virtual void removeComp(Component* component) = 0;

	const std::string& getName() const
	{
		return this->name;
	}

protected:
	std::string name;
};

#endif