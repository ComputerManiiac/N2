#include "Render2DComponent.h"
#include "Loader.h"


Render2DComponent::Render2DComponent(Entity* parent, ShaderProgram* shader, std::string texturePath)
	: Component("Render2D", parent)
{
	Loader::loadTGA(texturePath, textureID);
}



Render2DComponent::Render2DComponent()
{
}

Render2DComponent::~Render2DComponent()
{
}


const unsigned int& Render2DComponent::getTexID() const {
	return textureID;
}

ShaderProgram* Render2DComponent::getShader() const {
	return shader;
}