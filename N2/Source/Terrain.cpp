#include "Terrain.h"
#include "Manager.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "Primitives.h"
#include "Loader.h"

Terrain::Terrain(Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram * shader, std::string heightMapPath, std::vector<std::string> texturePaths){

	name = "terrain";

	TransformComponent* t; RenderComponent* r;
	contiguous.defineTypes(t, r);

	addComponent(contiguous.Allocate<TransformComponent>(this, position, rotation, scale));
	addComponent(contiguous.Allocate<RenderComponent>(this, shader, texturePaths[0]));


	Manager* manager = Manager::getInstance();
	manager->registerComponent<RenderSystem>(getComponent<RenderComponent>());

	OBJInfo info;
	Primitives::generateTerrain(info, heightMapPath, -10.0f, 10.0f, 0.5f);
	getComponent<RenderComponent>()->setInfo(info);

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
