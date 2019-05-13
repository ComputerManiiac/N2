#include "Manager.h"
#include "Application.h"
#include "UpdateSystem.h"
#include "Terrain.h"

Manager* Manager::instance = nullptr;



Manager::Manager()
{
	terrain = nullptr;
}


Manager * Manager::getInstance()
{
	if (instance == nullptr)
		instance = new Manager();
	return instance;
}

Manager::~Manager()
{

	for (auto& system : systems)
		delete system.second;

	for (auto& entity : entities)
		delete entity.second;

}

void Manager::Register() {
	registerSystem<UpdateSystem>();
	registerSystem<RenderSystem>();
	registerSystem<PhysicsSystem>();
	registerSystem<ParticleSystem>();
}

void Manager::Initialize()	
{

	getSystem<RenderSystem>()->Initialize();
	getSystem<ParticleSystem>()->Initialize();
	getSystem<PhysicsSystem>()->Initialize();

}

Terrain* Manager::getTerrain()
{
	return terrain;
}

Camera* Manager::getCamera()
{
	return &camera;
}

ShaderProgram* Manager::getShader(const std::string& name)
{
	return &shaders[name];
}


void Manager::Update(double dt)
{
	TransformComponent* transform = entities["sphere"]->getComponent<TransformComponent>();
	shaders["grass"].Use();
	shaders["grass"].setUniform("objPosition", transform->getPos());


	transform = entities["sphere"]->getComponent<TransformComponent>();

	if (Application::isKeyPressed(GLFW_KEY_I))
	{
		transform->Move(Vector3(0, 0, 2.0f) * dt);
	}

	if (Application::isKeyPressed(GLFW_KEY_K))
	{
		transform->Move(Vector3(0, 0, -2.0f) * dt);
	}

	if (Application::isKeyPressed(GLFW_KEY_J))
	{
		transform->Move(Vector3(2, 0, 0.0f) * dt);
	}

	if (Application::isKeyPressed(GLFW_KEY_L))
	{
		transform->Move(Vector3(-2, 0, 0.0f) * dt);
	}

	//RigidbodyComponent* rigidbody = entities["obj1"]->getComponent<RigidbodyComponent>();
	//
	//if (Application::isKeyPressed(GLFW_KEY_I))
	//{
	//	rigidbody->setVelocity(Vector3(0, 0, 2.0f));
	//}
	//if (Application::isKeyPressed(GLFW_KEY_K))
	//{
	//	rigidbody->setVelocity(Vector3(0, 0, -2.0f));
	//}

	//if (Application::isKeyPressed(GLFW_KEY_J))
	//{
	//	rigidbody->setVelocity(Vector3(-2.0f, 0, 0));
	//}

	//if (Application::isKeyPressed(GLFW_KEY_L))
	//{
	//	rigidbody->setVelocity(Vector3(2.0f, 0, 0));
	//}

	//if (Application::isKeyPressed(GLFW_KEY_U))
	//{
	//	rigidbody->setVelocity(Vector3(0, 2.0f, 0));
	//}

	//if (Application::isKeyPressed(GLFW_KEY_O))
	//{
	//	rigidbody->setVelocity(Vector3(0, -2.0f, 0));
	//}

	if (Application::isKeyPressDown(GLFW_KEY_3))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (Application::isKeyPressDown(GLFW_KEY_4))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (Application::isKeyPressed(GLFW_KEY_G))
		Application::framesPerSecond++;


	camera.Update(dt);

	getSystem<PhysicsSystem>()->Update(dt);
	getSystem<RenderSystem>()->Update(dt);
	getSystem<ParticleSystem>()->Update(dt);



}