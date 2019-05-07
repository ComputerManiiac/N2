#ifndef TERRAIN_H
#define TERRAIN_H

#include "Entity.h"



class Terrain : public Entity {

public:
	Terrain(Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram* shader, 
		std::string heightMapPath, std::vector<std::string> texturePaths, float minHeight, float maxHeight, float cellLength);
	Terrain();
	~Terrain();

	

	const unsigned int& getTexture(const unsigned int& index);

	float getHeight(const Vector2& position);


private:

	Vector2 convertToGrid(const Vector2& position);
	float getPixelValue(const Vector2& position);
	float scale;
	float gridLength;
	float minHeight;
	float maxHeight;
	float cellLength;
	std::vector<unsigned int> textures;
	std::vector<unsigned char> heightMapData;
};

#endif