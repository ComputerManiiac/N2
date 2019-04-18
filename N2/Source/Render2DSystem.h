#ifndef RENDER2DSYSTEM_H
#define RENDER2DSYSTEM_H

#include "System.h"
#include "Render2DComponent.h"
#include "Transform2DComponent.h"
#include "Info.h"
#include "Mtx44.h"
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

	Mtx44 projection;
	Mtx44 getTransformationMatrix(Transform2DComponent* comp);

	unsigned VAO, VBO, EBO;
	std::vector<Render2DComponent*> subscribers;
};

#endif