#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Manager.h"
#include "LevelEditor.h"

class SceneManager : public Manager
{
public:
	SceneManager();
	~SceneManager();

	void Initialize();
	void Update(double dt);

	static SceneManager* getInstance();


private:

	LevelEditor editor;
	bool editorMode;
	//std::map<std::string, Entity*> entities;
	Entity* entities[24][32];
	unsigned int entityCount;

};

#endif