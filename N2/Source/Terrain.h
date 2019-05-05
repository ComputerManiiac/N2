#ifndef TERRAIN_H
#define TERRAIN_H

#include "Entity.h"



class Terrain : public Entity {

public:
	Terrain(Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader, std::string heightMapPath, std::vector<std::string> texturePaths);
	Terrain();
	~Terrain();

	

	const unsigned int& getTexture(const unsigned int& index);

private:
	std::vector<unsigned int> textures;
};

#endif