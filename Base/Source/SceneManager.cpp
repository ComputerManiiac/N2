#include "SceneManager.h"
#include "Application.h"
#include "Primitives.h"
#include "Loader.h"
#include "Terrain.h"
#include "Skybox.h"
#include "UpdateSystem.h"
#include "Player.h"

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
	shaders.try_emplace("water", "Assets\\Shaders\\water.vert", "Assets\\Shaders\\water.geom", "Assets\\Shaders\\water.frag");

	//entities["ground"] = new Entity("ground", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(100, 1, 100), &shaders["lit"], "Assets\\Models\\cube.obj", "Assets\\Textures\\rock.tga");
	entities["sphere"] = new Player(Vector3(0.0f, 1.0f, 0.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["lit"], "Assets\\Models\\sphere.obj", "Assets\\Textures\\human.tga");
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



	OBJInfo skyplane;
	Primitives::generateSkyplane(skyplane, 128, 200, 2000, 1.0f, 1.0f);
	entities["skyplane"] = new Skybox(Vector3(500, 1200, -500), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["skybox"], 
		 { "Assets\\Textures\\Skybox\\top.tga", "Assets\\Textures\\night.tga" });
	entities["skyplane"]->getComponent<RenderComponent>()->setInfo(skyplane);

	terrain = new Terrain(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(2.0f, 1.0f, 2.0f), &shaders["terrain"],
		"Assets\\Textures\\heightmap.bmp",  { "Assets\\Textures\\terrain.tga","Assets\\Textures\\terrain_grass.tga" ,
		"Assets\\Textures\\terrain_mountain.tga",  "Assets\\Textures\\terrain_mud.tga",
		"Assets\\Textures\\terrain_path.tga", }, -20.0f, 25.0f, 1.0f);

	entities["terrain"] = terrain;

	entities["water"] = new Entity("water", Vector3(37.4f,-4.0f,-58.0f), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f), &shaders["lit"],
		"Assets\\Models\\cube.obj", "Assets\\Textures\\water.tga");

	OBJInfo water;
	Primitives::generateWater(water, 64.0f, 1.0f);
	entities["water"]->getComponent<RenderComponent>()->setInfo(water);

	entities["house"] = new Entity("house", Vector3(-10.0f, terrain->getHeight(Vector2(-10.0f, -30.0f)), -30.0f), Vector3(0, 0, -3.0f), Vector3(1.0f, 1.0f, 1.0f), &shaders["lit"],
		"Assets\\Models\\house.obj", "Assets\\Textures\\house.tga");

	spawnObjectOnTerrain("tree", 12, -0.2f, "Assets\\Models\\tree.obj", "Assets\\Textures\\tree.tga");
	spawnObjectOnTerrain("tree2", 12, -0.1f, "Assets\\Models\\tree2.obj", "Assets\\Textures\\tree2.tga");
	spawnObjectOnTerrain("rock", 8, 0.f, "Assets\\Models\\rock.obj", "Assets\\Textures\\rock.tga");
	//
	//for (int i = 0; i < 20; ++i)
	//{
	//	std::string treeName = "tree" + std::to_string(i);
	//	Vector2 position(Math::RandFloatMinMax(-64.0f, 64.f), Math::RandFloatMinMax(-64.0f, 64.f));
	//	float y = terrain->getHeight(position);
	//	std::cout << y << std::endl;
	//	entities[treeName] = new Entity(treeName, Vector3(position.x, y, position.y), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f), &shaders["lit"], "Assets\\Models\\tree.obj", "Assets\\Textures\\tree.tga");
	//}
	/*float y = terrain->getHeight(Vector2(-64, -64));
	std::cout << y << std::endl;
	entities["tree"] = new Entity("tree", Vector3(-64, y, -64), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f), &shaders["lit"], "Assets\\Models\\tree.obj", "Assets\\Textures\\tree.tga");*/
	
}

void SceneManager::spawnObjectOnTerrain(const std::string& type, const unsigned int& number, const float& offset,
	const std::string& modelPath, const std::string& texturePath)
{
	for (unsigned int n = 0; n < number; ++n) {
		float y;
		Vector2 position;
		do {
			position.Set(Math::RandFloatMinMax(-64.0f, 64.f), Math::RandFloatMinMax(-64.0f, 64.f));
			y = terrain->getHeight(position);
		} while (y < 0.0f);
		std::string name = type + std::to_string(n);
		entities[name] = new Entity(name, Vector3(position.x, terrain->getHeight(position) + offset, position.y), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f), &shaders["lit"], modelPath, texturePath);

	}
}

void SceneManager::Update(double dt)
{
	

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

	getSystem<UpdateSystem>()->Update(dt);
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
