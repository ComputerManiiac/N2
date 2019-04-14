#include "Entity.h"
#include "Manager.h"




Entity::Entity(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader, std::string texturePath) : name(name)
{
	TransformComponent* t; RenderComponent* r; ColliderComponent* c; RigidbodyComponent* rigid;
	contiguous.defineTypes(t, r, c, rigid);

	addComponent(contiguous.Allocate<TransformComponent>(this, position, rotation, scale));
	addComponent(contiguous.Allocate<RenderComponent>(this, shader, texturePath));
	addComponent(contiguous.Allocate<ColliderComponent>(this));
	addComponent(contiguous.Allocate<RigidbodyComponent>(this, 2.0f));

	Manager* manager = Manager::getInstance();
	manager->registerComponent<RenderSystem>(getComponent<RenderComponent>());
	manager->registerComponent<PhysicsSystem>(getComponent<RigidbodyComponent>());
}


Entity::Entity(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader, std::string modelPath, std::string texturePath) : name(name)
{
	TransformComponent* t; RenderComponent* r; ColliderComponent* c; RigidbodyComponent* rigid; ParticleComponent* particle;
	contiguous.defineTypes(t, r, c, rigid, particle);

	addComponent(contiguous.Allocate<TransformComponent>(this, position, rotation, scale));
	addComponent(contiguous.Allocate<RenderComponent>(this, shader, modelPath, texturePath));
	addComponent(contiguous.Allocate<ColliderComponent>(this));
	addComponent(contiguous.Allocate<RigidbodyComponent>(this, 2.0f));

	if(name != "ground")
		//addComponent(contiguous.Allocate<ParticleComponent>(this, true, 3.0f, 1.0f, Vector3(0, 0, 0)));
		addComponent(contiguous.Allocate<ParticleComponent>(this, true, 1.0f, 2.0f, Vector3(0,0,0)));
	//addComponent(contiguous.Allocate<ParticleComponent>(this));

	Manager* manager = Manager::getInstance();
	manager->registerComponent<RenderSystem>(getComponent<RenderComponent>());
	manager->registerComponent<PhysicsSystem>(getComponent<RigidbodyComponent>());

	if(name != "ground")
		manager->registerComponent<ParticleSystem>(getComponent<ParticleComponent>());

}

Entity::Entity(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader, bool doubleSided, bool collisionEnabled, std::string modelPath, std::string texturePath)
	: name(name)
{
	TransformComponent* t; RenderComponent* r; ColliderComponent* c; RigidbodyComponent* rigid;
	contiguous.defineTypes(t, r, c, rigid);

	addComponent(contiguous.Allocate<TransformComponent>(this, position, rotation, scale));
	addComponent(contiguous.Allocate<RenderComponent>(this, shader, doubleSided, modelPath, texturePath));

	Manager* manager = Manager::getInstance();
	manager->registerComponent<RenderSystem>(getComponent<RenderComponent>());

	if (collisionEnabled) {
		addComponent(contiguous.Allocate<ColliderComponent>(this));
		addComponent(contiguous.Allocate<RigidbodyComponent>(this, 2.0f));
		manager->registerComponent<PhysicsSystem>(getComponent<RigidbodyComponent>());
	}



}

Entity::Entity(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram * shader, bool collisionEnabled, std::string modelPath, std::string texturePath)
{

	TransformComponent* t; RenderComponent* r; ColliderComponent* c; RigidbodyComponent* rigid;
	contiguous.defineTypes(t, r, c, rigid);

	addComponent(contiguous.Allocate<TransformComponent>(this, position, rotation, scale));
	addComponent(contiguous.Allocate<RenderComponent>(this, shader, modelPath, texturePath));

	Manager* manager = Manager::getInstance();
	manager->registerComponent<RenderSystem>(getComponent<RenderComponent>());

	if (collisionEnabled)
	{
		addComponent(contiguous.Allocate<ColliderComponent>(this));
		addComponent(contiguous.Allocate<RigidbodyComponent>(this, 2.0f));
		manager->registerComponent<PhysicsSystem>(getComponent<RigidbodyComponent>());
	}
}


Entity::Entity(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader) : name(name)
{
	TransformComponent* t; RenderComponent* r; ColliderComponent* c; RigidbodyComponent* rigid;
	contiguous.defineTypes(t, r, c, rigid);

	addComponent(contiguous.Allocate<TransformComponent>(this, position, rotation, scale));
	addComponent(contiguous.Allocate<RenderComponent>(this, shader));
	addComponent(contiguous.Allocate<ColliderComponent>(this));
	addComponent(contiguous.Allocate<RigidbodyComponent>(this, 2.0f));

	Manager* manager = Manager::getInstance();
	manager->registerComponent<RenderSystem>(getComponent<RenderComponent>());
	manager->registerComponent<PhysicsSystem>(getComponent<RigidbodyComponent>());
}


Entity::Entity()
{

}


Entity::~Entity()
{

}

const std::string& Entity::getName() const
{
	return name;
}


