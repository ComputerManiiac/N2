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
	shaders.try_emplace("lit", "Assets\\Shaders\\lit.vert", "Assets\\Shaders\\lit.frag");
	shaders.try_emplace("depth", "Assets\\Shaders\\depth.vert", "Assets\\Shaders\\depth.frag");
	shaders.try_emplace("quad", "Assets\\Shaders\\quad.vert", "Assets\\Shaders\\quad.frag");
	shaders.try_emplace("ui", "Assets\\Shaders\\ui.vert", "Assets\\Shaders\\ui.frag");
	shaders.try_emplace("skybox", "Assets\\Shaders\\skybox.vert", "Assets\\Shaders\\skybox.frag");
	shaders.try_emplace("grass", "Assets\\Shaders\\grass.vert", "Assets\\Shaders\\grass.frag");
	shaders.try_emplace("particle", "Assets\\Shaders\\particle.vert", "Assets\\Shaders\\particle.frag");

	registerSystem<RenderSystem>(); 
	registerSystem<PhysicsSystem>();
	registerSystem<ParticleSystem>();


	entities["ground"] = new Entity("ground", Vector3(0, 0, 0), Vector3(0,0,0), Vector3(100,1,100), &shaders["lit"], "Assets\\Models\\cube.obj", "Assets\\Textures\\rock.tga");
	
	
	//float offsetX = 2.0f;

	//for (int x = 0; x < 50; x++)
	//{
	//	float offsetZ = 5.0f;
	//	for (int z = 0; z < 50; z++)
	//	{
	//		std::string name = "grass" + std::to_string(x) + "-" + std::to_string(z);
	//		entities[name] = new Entity(name, Vector3(offsetX, 1.0f, offsetZ), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["grass"], false, "Assets\\Models\\grass.obj", "Assets\\Textures\\grass.tga");
	//		offsetZ += 0.4f;
	//	}
	//	offsetX += 0.6f;
	//}
	

	/*entities["car"] = new Entity("car", Vector3(0.f, 3.f, 0.f), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["lit"], "Assets\\Models\\devastator.obj", "Assets\\Textures\\devastator.tga");*/
	entities["sphere"] = new Entity("sphere", Vector3(0.0f, 1.0f, 0.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["lit"], "Assets\\Models\\sphere.obj", "Assets\\Textures\\human.tga");
	entities["particleSphere"] = new Entity("particleSphere", Vector3(5.0f, 2.0f, 0.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["lit"], "Assets\\Models\\sphere.obj", "Assets\\Textures\\human.tga");


	getSystem<RenderSystem>()->Initialize();
	getSystem<ParticleSystem>()->Initialize();
	getSystem<PhysicsSystem>()->Initialize();


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

	if (Application::isKeyPressDown(GLFW_KEY_H))
		getSystem<RenderSystem>()->renderSkybox = !getSystem<RenderSystem>()->renderSkybox;

	camera.Update(dt);
	getSystem<PhysicsSystem>()->Update(dt);
	getSystem<RenderSystem>()->Update(dt);
	getSystem<ParticleSystem>()->Update(dt);

}