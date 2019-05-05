#include "SceneManager.h"
#include "Application.h"
#include "Primitives.h"
#include "Loader.h"
#include "Terrain.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	Manager::Register();

	camera = Camera(Vector3(0, 0, 0));
	shaders.try_emplace("lit", "Assets\\Shaders\\lit.vert", "Assets\\Shaders\\lit.geom", "Assets\\Shaders\\lit.frag");
	shaders.try_emplace("depth", "Assets\\Shaders\\depth.vert", "Assets\\Shaders\\depth.frag");
	shaders.try_emplace("quad", "Assets\\Shaders\\quad.vert", "Assets\\Shaders\\quad.frag");
	shaders.try_emplace("ui", "Assets\\Shaders\\ui.vert", "Assets\\Shaders\\ui.frag");
	shaders.try_emplace("skybox", "Assets\\Shaders\\skybox.vert", "Assets\\Shaders\\skybox.frag");
	shaders.try_emplace("grass", "Assets\\Shaders\\grass.vert", "Assets\\Shaders\\grass.frag");
	shaders.try_emplace("particle", "Assets\\Shaders\\particle.vert", "Assets\\Shaders\\particle.frag");
	shaders.try_emplace("terrain", "Assets\\Shaders\\terrain.vert", "Assets\\Shaders\\terrain.geom", "Assets\\Shaders\\terrain.frag");

	//entities["ground"] = new Entity("ground", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(100, 1, 100), &shaders["lit"], "Assets\\Models\\cube.obj", "Assets\\Textures\\rock.tga");
	entities["sphere"] = new Entity("sphere", Vector3(0.0f, 1.0f, 0.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["lit"], "Assets\\Models\\sphere.obj", "Assets\\Textures\\human.tga");
	//entities["particleSphere"] = new Entity("particleSphere", Vector3(5.0f, 2.0f, 0.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["lit"], "Assets\\Models\\sphere.obj", "Assets\\Textures\\human.tga");

	//float offsetX = 2.0f;

	//for (int x = 0; x < 50; x++)
	//{
	//	float offsetZ = 5.0f;
	//	for (int z = 0; z < 10; z++)
	//	{
	//		std::string name = "grass" + std::to_string(x) + "-" + std::to_string(z);
	//		entities[name] = new Entity(name, Vector3(offsetX, 1.0f, offsetZ), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["grass"], false, "Assets\\Models\\grass.obj", "Assets\\Textures\\grass.tga");
	//		offsetZ += 0.4f;
	//	}
	//	offsetX += 0.6f;
	//}

	createScene();

	Manager::Initialize();

}

void SceneManager::createScene()
{
	for (int i = 0; i < 10; ++i)
	{
		std::string treeName = "tree" + std::to_string(i);
		entities[treeName] = new Entity(treeName, Vector3(Math::RandFloatMinMax(-320.0f, 320.f), 0, Math::RandFloatMinMax(-320.0f, 320.f)), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f), &shaders["lit"], "Assets\\Models\\tree.obj", "Assets\\Textures\\tree.tga");
	}


	OBJInfo skyplane;
	Primitives::generateSkyplane(skyplane, 128, 200, 2000, 1.0f, 1.0f);
	entities["skyplane"] = new Entity("skyplane", Vector3(500, 1200, -500), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["lit"], "Assets\\Models\\cube.obj", "Assets\\Textures\\Skybox\\top.tga");
	entities["skyplane"]->getComponent<RenderComponent>()->setInfo(skyplane);

	entities["terrain"] = new Terrain(Vector3(0, -10, 0), Vector3(0, 0, 0), Vector3(8.0f, 3.0f, 8.0f), &shaders["terrain"], 
		"Assets\\Textures\\heightmap.bmp",  { "Assets\\Textures\\terrain.tga","Assets\\Textures\\terrain_grass.tga" ,
		"Assets\\Textures\\terrain_mountain.tga",  "Assets\\Textures\\terrain_mud.tga",
		"Assets\\Textures\\terrain_path.tga", });

	
}

void SceneManager::Update(double dt)
{
	//TransformComponent* transform = entities["sphere"]->getComponent<TransformComponent>();
	//shaders["grass"].Use();
	//shaders["grass"].setUniform("objPosition", transform->getPos());


	//transform = entities["sphere"]->getComponent<TransformComponent>();

	/*if (Application::isKeyPressed(GLFW_KEY_I))
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
	}*/


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

SceneManager* SceneManager::getInstance()
{
	if (instance == nullptr)
		instance = new SceneManager();
	return static_cast<SceneManager*>(instance);
}
