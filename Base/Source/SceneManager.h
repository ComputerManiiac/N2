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

	void createScene();
	void spawnObjectOnTerrain(const std::string& type, const unsigned int& number, const float& offset, const std::string& modelPath, const std::string& texturePath);
};

#endif