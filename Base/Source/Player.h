#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity
{
public:

	Player(Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader, std::string modelPath, std::string texturePath);
	Player();
	~Player();

	void Update(double& dt);
};

#endif