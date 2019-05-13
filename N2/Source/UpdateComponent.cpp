#include "UpdateComponent.h"



UpdateComponent::UpdateComponent(Entity * parent) : Component("UpdateComponent", parent)
{
}

UpdateComponent::UpdateComponent()
{
}


UpdateComponent::~UpdateComponent()
{
}


void UpdateComponent::Update(double& dt)
{
	if (function)
		function(dt);
}