#include "UpdateSystem.h"
#include <algorithm>


UpdateSystem::UpdateSystem()
{
}


UpdateSystem::~UpdateSystem()
{
}

void UpdateSystem::Initialize()
{
}

void UpdateSystem::Update(double& dt)
{
	for (auto it = subscribers.begin(); it < subscribers.end(); ++it)
		static_cast<UpdateComponent*>(*it)->Update(dt);
}

void UpdateSystem::registerComp(Component * component)
{
	UpdateComponent* update = static_cast<UpdateComponent*>(component);
	if (update == nullptr) return;
	subscribers.push_back(update);
}

void UpdateSystem::removeComp(Component * component)
{
	UpdateComponent* update = static_cast<UpdateComponent*>(component);
	if (update == nullptr) return;
	subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), update));
}
