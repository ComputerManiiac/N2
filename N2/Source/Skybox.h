#ifndef SKYBOX_H
#define SKYBOX_H

#include "Entity.h"


class Skybox : public Entity {

public:
	Skybox(Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader, std::vector<std::string> texturePaths);
	Skybox();
	~Skybox();

	const unsigned int& getTexture(const unsigned int& index);

private:
	std::vector<unsigned int> textures;
};

#endif