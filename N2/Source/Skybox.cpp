#include "Skybox.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Loader.h"
#include "Manager.h"

Skybox::Skybox(Vector3 position, Vector3 rotation, Vector3 scale, ShaderProgram * shader, 
	std::vector<std::string> texturePaths)
{

	name = "skybox";

	TransformComponent* t; RenderComponent* r;
	contiguous.defineTypes(t, r);

	addComponent(contiguous.Allocate<TransformComponent>(this, position, rotation, scale));
	addComponent(contiguous.Allocate<RenderComponent>(this, shader, texturePaths[0]));


	Manager* manager = Manager::getInstance();
	manager->registerComponent<RenderSystem>(getComponent<RenderComponent>());

	for (int i = 1; i < (int)texturePaths.size(); ++i)
	{
		const std::string texturePath = texturePaths[i];
		unsigned int textureID;
		Loader::loadTGA(texturePath, textureID);
		textures.push_back(textureID);
	}
}

Skybox::Skybox() {

}

Skybox::~Skybox() {

}

const unsigned int & Skybox::getTexture(const unsigned int & index)
{
	return textures[index - 1];
}
