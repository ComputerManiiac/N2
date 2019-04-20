#include "LevelEditor.h"
#include "Application.h"
#include "Transform2DComponent.h"
#include "Render2DComponent.h"
#include "Manager.h"
#include "Render2DSystem.h"

LevelEditor::LevelEditor()
{
}


LevelEditor::~LevelEditor()
{
	if(tileMap != nullptr)
		delete tileMap;
	if(selection != nullptr)
		delete selection;
}

void LevelEditor::Initialize()
{
	tileShader = Manager::getInstance()->getShader("tile");


	tileMap = new Entity2D("tileMap", Vector3(0, 0, 0), 0, Vector2(512.0f,
		512.0f), tileShader, "Assets\\Textures\\2D\\tileset.tga",
		Vector2(0, 0),
		Vector2(512, 512));

	textureOffset.x = 0.0f;
	textureOffset.y = 1.0f - (1.0f / 32.0f);

	selection = new Entity2D("selection", Vector3(0, 0, 0), 0, Vector2(25, 25), tileShader, "Assets\\Textures\\2D\\tileset.tga", textureOffset, Vector2(16, 16));
	selection->getComponent<Render2DComponent>()->setActive(false);
	defineTileMode = true;
}


void LevelEditor::Update(Entity* entities[24][32], unsigned int& entityCount)
{
	//Manager::getInstance()->getSystem<Render2DSystem>()->renderText("Select Tile: Single Click/Drag Selection", 190, 500, "sansserif", Vector3(1, 1, 1), 0.30f);
	//Manager::getInstance()->getSystem<Render2DSystem>()->renderText("Tile Selection: Tab", 100, 200, "sansserif", Vector3(1, 1, 1), 0.30f);
	//Manager::getInstance()->getSystem<Render2DSystem>()->renderText("Place Tile: Left Click/Hold", 270, 420, "sansserif", Vector3(1, 1, 1), 0.30f);
	//Manager::getInstance()->getSystem<Render2DSystem>()->renderText("Remove Tile: Right Click/Hold", 250, 450, "sansserif", Vector3(1, 1, 1), 0.30f);
	//Manager::getInstance()->getSystem<Render2DSystem>()->renderText("Remove All Tiles: C", 285, 480, "sansserif", Vector3(1, 1, 1), 0.30f);
	//Manager::getInstance()->getSystem<Render2DSystem>()->renderText("Play: Enter", 340, 510, "sansserif", Vector3(1, 1, 1), 0.30f);

	if (defineTileMode)
	{
		

		int x, y;
		getTileAtCursor(x, y, 16.0f);


		if (Application::isKeyPressDown(GLFW_MOUSE_BUTTON_LEFT))
		{

			start.x = x;
			start.y = y;
		}

		if (Application::isKeyPressRelease(GLFW_MOUSE_BUTTON_LEFT))
		{

			end.x = x;
			end.y = y;

			Vector2 smaller;
			smaller.x = min(start.x, end.x);
			smaller.y = min(start.y, end.y);

			Vector2 larger;
			larger.x = max(start.x, end.x);
			larger.y = max(start.y, end.y);
			
			int blockDiffX = abs(larger.x - smaller.x + 1);
			int blockDiffY = abs(larger.y - smaller.y + 1);


			textureOffset.x = (smaller.x / 32.0f);
			textureOffset.y = 1.0f - ((smaller.y + blockDiffY) / 32.0f);

			textureSize.x = blockDiffX * 16;
			textureSize.y = blockDiffY * 16;

			std::cout << "Block Diff X: " << blockDiffX << std::endl;
			std::cout << "Block Diff Y: " << blockDiffY << std::endl;

			selection->getComponent<Transform2DComponent>()->setScale(Vector2(blockDiffX * 25, blockDiffY * 25));
			selection->getComponent<Render2DComponent>()->setTextureSize(textureSize);
			selection->getComponent<Render2DComponent>()->setTextureOffset(textureOffset);
			
		}

		

	}
	else
	{

		int x, y;
		getTileAtCursor(x, y, 25.0f);



		selection->getComponent<Transform2DComponent>()->setPosition(25.0f * Vector3(x, y, 0));
	

		if (Application::isKeyPressed(GLFW_MOUSE_BUTTON_LEFT))
		{

			if (entities[y][x] == nullptr)
			{
				entities[y][x] = new Entity2D(std::to_string(glfwGetTime()), Vector3(x * 25.0f, y * 25.0f, 0), 0, selection->getComponent<Transform2DComponent>()->getScale(),
					tileShader, "Assets\\Textures\\2D\\tileset.tga", textureOffset, textureSize);
				++entityCount;
			}
		}

		else if (Application::isKeyPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{

			if (y < 24 && x < 32)
			{
				if (entities[y][x] != nullptr)
				{
					delete entities[y][x];
					entities[y][x] = nullptr;
					--entityCount;
				}
			}
		}
		else if (Application::isKeyPressDown(GLFW_KEY_C))
		{
			for (int y = 0; y < 24; ++y)
			{
				for (int x = 0; x < 32; ++x)
				{
					if (entities[y][x] != nullptr)
					{
						delete entities[y][x];
						entities[y][x] = nullptr;
						--entityCount;
					}
				}
			}
		}

		// Leave editor mode
		if (Application::isKeyPressDown(GLFW_KEY_ENTER))
		{
			selection->getComponent<Render2DComponent>()->setActive(false);
		}


	}


	if (Application::isKeyPressDown(GLFW_KEY_TAB) || (defineTileMode && Application::isKeyPressRelease(GLFW_MOUSE_BUTTON_LEFT)))
	{
		defineTileMode = !defineTileMode;
		for (int y = 0; y < 24; ++y)
		{
			for (int x = 0; x < 32; ++x)
			{
				if (entities[y][x] != nullptr)
					entities[y][x]->getComponent<Render2DComponent>()->setActive(!defineTileMode);
			}
		}
		selection->getComponent<Render2DComponent>()->setActive(!defineTileMode);
		tileMap->getComponent<Render2DComponent>()->setActive(defineTileMode);
	}


}

void LevelEditor::getTileAtCursor(int& x, int& y, float divisor)
{
	x = (int)floor(Application::getMouseX() / divisor);
	y = (int)floor(Application::getMouseY() / divisor);
}
