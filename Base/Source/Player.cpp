#include "Player.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include "UpdateComponent.h"
#include "Manager.h"
#include "Application.h"
#include "UpdateSystem.h"



Player::Player(Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram * shader, std::string modelPath, std::string texturePath)
{
	this->name = "player";

	TransformComponent* t; RenderComponent* r; ColliderComponent* c; UpdateComponent* u;
	contiguous.defineTypes(t, r, c, u);

	addComponent(contiguous.Allocate<TransformComponent>(this, position, rotation, scale));
	addComponent(contiguous.Allocate<RenderComponent>(this, shader, modelPath, texturePath));
	addComponent(contiguous.Allocate<ColliderComponent>(this));
	addComponent(contiguous.Allocate<UpdateComponent>(this));

	Manager* manager = Manager::getInstance();
	manager->registerComponent<RenderSystem>(getComponent<RenderComponent>());
	manager->registerComponent<UpdateSystem>(getComponent<UpdateComponent>());

	getComponent<UpdateComponent>()->bindFunction(&Player::Update, this);
}

Player::Player()
{
}


Player::~Player()
{
}


void Player::Update(double& dt)
{
	float float_dt = static_cast<float>(dt);

	TransformComponent* transform = getComponent<TransformComponent>();
	Camera* camera = Manager::getInstance()->getCamera();
	Vector3 position = transform->getPos();
	Vector3 front = camera->getFront();
	front.y = 0.0f;
	Vector3 right = camera->getRight();

	const float cameraSpeed = 5.0f;

	if (Application::isKeyPressed(GLFW_KEY_W))
	{
		position += front * cameraSpeed * float_dt;
		//Manager::getInstance()->getCamera()->Move(front * 5.0f * dt);
	}

	if (Application::isKeyPressed(GLFW_KEY_A))
	{
		position += -right * cameraSpeed * float_dt;
	}

	if (Application::isKeyPressed(GLFW_KEY_S))
	{
		position += -front * cameraSpeed * float_dt;
	
	}

	if (Application::isKeyPressed(GLFW_KEY_D))
	{
		position += right * cameraSpeed * float_dt;
	}

	float targetY = Manager::getInstance()->getTerrain()->getHeight(Vector2(position.x, position.z)) + 0.5f;
	position.y = Math::Lerp(position.y, targetY, 4.0f * float_dt);

	transform->setPos(position);
	Manager::getInstance()->getCamera()->setPosition(position + Vector3(0.0f, 1.0f, 0.0f));
}