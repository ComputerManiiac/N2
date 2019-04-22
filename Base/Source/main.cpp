#include "GameApplication.h"
#include "SceneManager.h"
#include <typeindex>
#include <map>
#include <iostream>

int main()
{

	SceneManager* manager = SceneManager::getInstance();
	GameApplication app("N2 Engine", 1368, 768);
	app.setManager(manager);
	app.Run();


	MemoryLeak::Dump();
	return 0;
}