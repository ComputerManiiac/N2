#ifndef UPDATECOMPONENT_H
#define UPDATECOMPONENT_H

#include "Component.h"
#include <functional>

class UpdateComponent : public Component
{
public:

	UpdateComponent(Entity* parent);
	UpdateComponent();
	~UpdateComponent();


	template<typename Function, typename Context>
	void bindFunction(Function&& function, Context&& context)
	{
		this->function = std::bind(function, context, std::placeholders::_1);
	}

	void Update(double& dt);

private:
	std::function<void(double&)> function;
};

#endif