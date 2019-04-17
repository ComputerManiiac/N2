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
};

#endif