#include "Entity2D.h"
#include "Manager.h"
#include "Transform2DComponent.h"
#include "Render2DSystem.h"

Entity2D::Entity2D(std::string name, Vector3 position, float rotation, Vector2 scale, ShaderProgram* shader, std::string texturePath, Vector2 textureOffset, Vector2 textureSize) 
 : Entity(name){

	Transform2DComponent* t; Render2DComponent* r;
	contiguous.defineTypes(t, r);

	addComponent(contiguous.Allocate<Transform2DComponent>(this, position, rotation, scale));
	addComponent(contiguous.Allocate<Render2DComponent>(this, shader, texturePath, textureOffset, textureSize));

	Manager* manager = Manager::getInstance();
	
	manager->registerComponent<Render2DSystem>(getComponent<Render2DComponent>());
	//TransformComponent* t; RenderComponent* r; ColliderComponent* c; RigidbodyComponent* rigid;
	//contiguous.defineTypes(t, r, c, rigid);

	//addComponent(contiguous.Allocate<TransformComponent>(this, position, rotation, scale));
	//addComponent(contiguous.Allocate<RenderComponent>(this, shader, texturePath));
	//addComponent(contiguous.Allocate<ColliderComponent>(this));
	//addComponent(contiguous.Allocate<RigidbodyComponent>(this, 2.0f));

	//Manager* manager = Manager::getInstance();
	//manager->registerComponent<RenderSystem>(getComponent<RenderComponent>());
	//manager->registerComponent<PhysicsSystem>(getComponent<RigidbodyComponent>());
}

Entity2D::Entity2D()
{
}


Entity2D::~Entity2D()
{
	Manager* manager = Manager::getInstance();
	manager->getSystem<Render2DSystem>()->removeComp(getComponent<Render2DComponent>());
}
