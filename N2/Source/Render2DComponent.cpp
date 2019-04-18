#include "Render2DComponent.h"
#include "Loader.h"


Render2DComponent::Render2DComponent(Entity* parent, ShaderProgram* shader, std::string texturePath, Vector2 textureOffset)
	: Component("Render2D", parent), shader(shader), textureOffset(textureOffset)
{
	Loader::loadTGA(texturePath, textureID, GL_NEAREST);
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

const Vector2& Render2DComponent::getTextureOffset() const
{
	return textureOffset;
}

ShaderProgram* Render2DComponent::getShader() const {
	return shader;
}