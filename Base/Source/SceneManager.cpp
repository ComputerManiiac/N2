#include "SceneManager.h"
#include "Application.h"
#include "Render2DSystem.h"
#include "Entity2D.h"
#include "Camera2D.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{



	for (int y = 0; y < 24; ++y)
	{
		for (int x = 0; x < 32; ++x)
		{
			if(entities[y][x] != nullptr)
				delete entities[y][x];
		}
	}

	Manager::~Manager();
}

void SceneManager::Initialize()
{
	editorMode = true;
	entityCount = 0;

	for (int y = 0; y < 24; ++y)
	{
		for (int x = 0; x < 32; ++x)
		{
			entities[y][x] = nullptr;
		}
	}

	camera = new Camera2D(Vector3(0, 0, -1));

	Application::toggleCursorLock(GLFW_CURSOR_NORMAL);

	registerSystem<Render2DSystem>();
	shaders.try_emplace("tile", "Assets\\Shaders\\2D\\tile.vert", "Assets\\Shaders\\2D\\tile.frag");
	shaders.try_emplace("ui", "Assets\\Shaders\\2D\\ui.vert", "Assets\\Shaders\\2D\\ui.frag");
	shaders.try_emplace("quad", "Assets\\Shaders\\2D\\quad.vert", "Assets\\Shaders\\2D\\quad.frag");
	
	
	editor.Initialize();

	getSystem<Render2DSystem>()->Initialize();

	//background = new Entity2D("background", Vector3(0, 0, -1), 0, Vector2(800, 600), &shaders["tile"], "Assets\\Textures\\2D\\background.tga", Vector2(0,0), Vector2();
	//entities["house"] = new Entity2D("house", Vector3(400, 300, 1), 0, Vector2(75, 100), &shaders["tile"], "Assets\\Textures\\2D\\house.tga");
	//entities["tree2"] = new Entity2D("tree2", Vector3(420, 300, 0), 0, Vector2(75, 75), &shaders["tile"], "Assets\\Textures\\2D\\tree.tga");

	//entities["testblock"] = new Entity2D("testblock", Vector3(0, 0, 0),0, Vector2(75, 75), &shaders["tile"], "Assets\\Textures\\2D\\tree.tga");

	//entities["grass"] = new Entity2D("grass", Vector3(0, 0, 0), 0, Vector2(25, 25), &shaders["tile"], "Assets\\Textures\\2D\\tileset.tga", Vector2(6.0f / 32.0f, 1.0f - (1.0f / 32.0f)));
}

void SceneManager::Update(double dt)
{
	if (editorMode)
	{
		editor.Update(entities, entityCount);
		if (Application::isKeyPressDown(GLFW_KEY_ENTER))
		{
			editorMode = false;
		}
	}


	if (Application::isKeyPressDown(GLFW_KEY_P))
		Application::toggleCursorLock(GLFW_CURSOR_DISABLED);
	if (Application::isKeyPressDown(GLFW_KEY_O))
		Application::toggleCursorLock(GLFW_CURSOR_NORMAL);


	getSystem<Render2DSystem>()->renderText("FPS: " + std::to_string(Application::framesPerSecond), 10, 530, "sansserif", Vector3(1, 1, 0), 0.30f);
	getSystem<Render2DSystem>()->renderText("Tile Count: " + std::to_string(entityCount), 10, 560, "sansserif", Vector3(1, 0, 0), 0.30f);

	camera->Update(dt);
	getSystem<Render2DSystem>()->Update(dt);
	

}

SceneManager* SceneManager::getInstance()
{
	if (instance == nullptr)
		instance = new SceneManager();
	return static_cast<SceneManager*>(instance);
}


/*Manager::Register();

camera = Camera(Vector3(0, 1, 5));
shaders.try_emplace("lit", "Assets\\Shaders\\lit.vert", "Assets\\Shaders\\lit.frag");
shaders.try_emplace("depth", "Assets\\Shaders\\depth.vert", "Assets\\Shaders\\depth.frag");
shaders.try_emplace("quad", "Assets\\Shaders\\quad.vert", "Assets\\Shaders\\quad.frag");
shaders.try_emplace("ui", "Assets\\Shaders\\ui.vert", "Assets\\Shaders\\ui.frag");
shaders.try_emplace("skybox", "Assets\\Shaders\\skybox.vert", "Assets\\Shaders\\skybox.frag");
shaders.try_emplace("grass", "Assets\\Shaders\\grass.vert", "Assets\\Shaders\\grass.frag");
shaders.try_emplace("particle", "Assets\\Shaders\\particle.vert", "Assets\\Shaders\\particle.frag");

entities["ground"] = new Entity("ground", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(100, 1, 100), &shaders["lit"], "Assets\\Models\\cube.obj", "Assets\\Textures\\rock.tga");
entities["sphere"] = new Entity("sphere", Vector3(0.0f, 1.0f, 0.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["lit"], "Assets\\Models\\sphere.obj", "Assets\\Textures\\human.tga");
entities["particleSphere"] = new Entity("particleSphere", Vector3(5.0f, 2.0f, 0.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), &shaders["lit"], "Assets\\Models\\sphere.obj", "Assets\\Textures\\human.tga");*/

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


/*TransformComponent* transform = entities["sphere"]->getComponent<TransformComponent>();
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
getSystem<ParticleSystem>()->Update(dt);*/