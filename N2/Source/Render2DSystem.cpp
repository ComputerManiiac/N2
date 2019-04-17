#include "Render2DSystem.h"
#include <algorithm>


Render2DSystem::Render2DSystem()
{
}


Render2DSystem::~Render2DSystem()
{
}

void Render2DSystem::Initialize()
{
	std::cout << "Initialized" << std::endl;
}

void Render2DSystem::Update(double& dt)
{
}

void Render2DSystem::registerComp(Component* component)
{
	Render2DComponent* render = static_cast<Render2DComponent*>(component);
	if (render == nullptr) return;
	subscribers.push_back(render);
}

void Render2DSystem::removeComp(Component* component)
{
	Render2DComponent* render = static_cast<Render2DComponent*>(component);
	if (render == nullptr) return;

	subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), render));

	//BatchKey key(render->getShader(), render->getTexID(), render->getMaterial());
	//if (batches.find(key) != batches.end())
	//{
	//	std::vector<RenderComponent*>* subs = &batches[key].subscribers;
	//	auto pos = std::find(subs->begin(), subs->end(), render);
	//	if (pos != subs->end())
	//		subs->erase(pos);
	//}
}
