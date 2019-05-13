#ifndef UPDATESYSTEM_H
#define UPDATESYSTEM_H

#include "System.h"
#include <vector>
#include "UpdateComponent.h"

class UpdateSystem : public System
{
public:
	UpdateSystem();
	~UpdateSystem();

	void Initialize();
	void Update(double& dt);

	void registerComp(Component* component);
	void removeComp(Component* component);


private:
	std::vector<UpdateComponent*> subscribers;
};

#endif