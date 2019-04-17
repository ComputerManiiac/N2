#ifndef RENDER2DSYSTEM_H
#define RENDER2DSYSTEM_H

#include "System.h"
#include "Render2DComponent.h"
#include <vector>

class Render2DSystem : public System
{
public:
	Render2DSystem();
	~Render2DSystem();

	void Initialize();
	void Update(double& dt);

	void registerComp(Component* component);
	void removeComp(Component* component);

private:
	std::vector<Render2DComponent*> subscribers;
};

#endif