#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Manager.h"

class SceneManager : public Manager
{
public:
	SceneManager();
	~SceneManager();

	void Initialize();
	void Update(double dt);

	static SceneManager* getInstance();

private:
	bool editorMode;
	bool defineTileMode;
	int currentObject;
	//std::map<std::string, Entity*> entities;
	Entity* entities[24][32];
	Entity* tileMap;
	Entity* selection;
	Vector2 textureOffset;
};

#endif