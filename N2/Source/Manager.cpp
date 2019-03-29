#include "Manager.h"
#include "Application.h"


Manager* Manager::instance = nullptr;



Manager::Manager()
{
	
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

void Manager::Initialize()
{

	camera = Camera(Vector3(0, 1, 5));
	shaders.try_emplace("lit", ShaderProgram("Assets\\Shaders\\lit.vert", "Assets\\Shaders\\lit.frag"));

	registerSystem<RenderSystem>(); 
	registerSystem<PhysicsSystem>();
	
	entities["ground"] = new Entity("ground", Vector3(0, 0, 0), Vector3(0,0,0), Vector3(100,1,100));
	

	//for (int x = 1; x < 11; x += 2)
	//{


	//}
	
	for (int i = 1; i < 3; ++i)
	{
		std::string name = "obj" + std::to_string(i);
		entities[name] = new Entity(name, Vector3(1, i * 2.0f, 1), Vector3(0, 0, 0), Vector3(1, 1, 1));
	}

	
	for (auto& system : systems) {
		system.second->Initialize();
	}





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

	RigidbodyComponent* rigidbody = entities["obj1"]->getComponent<RigidbodyComponent>();
	
	if (Application::isKeyPressed(GLFW_KEY_I))
	{
		rigidbody->setVelocity(Vector3(0, 0, 2.0f));
	}
	if (Application::isKeyPressed(GLFW_KEY_K))
	{
		rigidbody->setVelocity(Vector3(0, 0, -2.0f));
	}

	if (Application::isKeyPressed(GLFW_KEY_J))
	{
		rigidbody->setVelocity(Vector3(-2.0f, 0, 0));
	}

	if (Application::isKeyPressed(GLFW_KEY_L))
	{
		rigidbody->setVelocity(Vector3(2.0f, 0, 0));
	}

	if (Application::isKeyPressed(GLFW_KEY_U))
	{
		rigidbody->setVelocity(Vector3(0, 2.0f, 0));
	}

	if (Application::isKeyPressed(GLFW_KEY_O))
	{
		rigidbody->setVelocity(Vector3(0, -2.0f, 0));
	}



	camera.Update(dt);
	getSystem<PhysicsSystem>()->Update(dt);
	getSystem<RenderSystem>()->Update(dt);

}