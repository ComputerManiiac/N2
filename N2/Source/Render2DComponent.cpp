#include "Render2DComponent.h"
#include "Loader.h"


Render2DComponent::Render2DComponent(Entity* parent, ShaderProgram* shader, std::string texturePath, Vector2 textureOffset, Vector2 textureSize)
	: Component("Render2D", parent), shader(shader), textureOffset(textureOffset), textureSize(textureSize)
{
	Loader::loadTGA(texturePath, textureID, GL_NEAREST);
}



Render2DComponent::Render2DComponent()
{
}

Render2DComponent::~Render2DComponent()
{
}

void Render2DComponent::setTextureOffset(const Vector2& textureOffset)
{
	this->textureOffset = textureOffset;
}


const unsigned int& Render2DComponent::getTexID() const {
	return textureID;
}

const Vector2& Render2DComponent::getTextureSize() const
{
	return textureSize;
}

const Vector2& Render2DComponent::getTextureOffset() const
{
	return textureOffset;
}

ShaderProgram* Render2DComponent::getShader() const {
	return shader;
}