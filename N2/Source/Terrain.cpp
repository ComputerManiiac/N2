#include "Terrain.h"
#include "Manager.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Primitives.h"
#include "Loader.h"

Terrain::Terrain(Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram * shader, std::string heightMapPath, 
	std::vector<std::string> texturePaths, float minHeight, float maxHeight, float cellLength) 
	: minHeight(minHeight), maxHeight(maxHeight), cellLength(cellLength){

	name = "terrain";

	this->scale = scale.x;

	TransformComponent* t; RenderComponent* r;
	contiguous.defineTypes(t, r);

	addComponent(contiguous.Allocate<TransformComponent>(this, position, rotation, scale));
	addComponent(contiguous.Allocate<RenderComponent>(this, shader, texturePaths[0]));


	Manager* manager = Manager::getInstance();
	manager->registerComponent<RenderSystem>(getComponent<RenderComponent>());

	OBJInfo info;
	Primitives::generateTerrain(info, heightMapData, heightMapPath, minHeight, maxHeight, cellLength);
	getComponent<RenderComponent>()->setInfo(info);

	Material mat = getComponent<RenderComponent>()->getMaterial();
	mat.specular.Set(0.5f, 0.5f, 0.5f);
	getComponent<RenderComponent>()->setMaterial(mat);


	gridLength = static_cast<float>(sqrt(heightMapData.size() / 3));

	for (int i = 1; i < (int)texturePaths.size(); ++i)
	{
		const std::string texturePath = texturePaths[i];
		unsigned int textureID;
		Loader::loadTGA(texturePath, textureID);
		textures.push_back(textureID);
	}
}


Terrain::Terrain()
{


}

Terrain::~Terrain()
{

}

const unsigned int & Terrain::getTexture(const unsigned int& index)
{
	return textures[index - 2];
}

Vector2 Terrain::convertToGrid(const Vector2& position) {
	float halfGridLength = gridLength * 0.5f * cellLength;

	Vector2 start(-halfGridLength, -halfGridLength);

	Vector2 grid;
	grid.x = position.x / scale - start.x;
	grid.y = position.y / scale - start.y;
	grid = grid * (1.0f / cellLength);
	grid.x = static_cast<int>(grid.x);
	grid.y = static_cast<int>(grid.y);

	return grid;
}

float Terrain::getPixelValue(const Vector2& grid) {

	int pixel = (grid.y * gridLength + grid.x) * 3;
	float averageColorOfPixel = (heightMapData[pixel] + heightMapData[pixel + 1] + heightMapData[pixel + 2]) / 3.0f;

	return minHeight + (averageColorOfPixel / 255.0f) * (maxHeight - minHeight);
}

float Terrain::getHeight(const Vector2& position) {

	Vector2 grid = convertToGrid(position);
	return getPixelValue(grid);
}


