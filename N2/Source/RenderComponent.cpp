#include "RenderComponent.h"
#include "Loader.h"
#include "Primitives.h"
#include "Entity.h"

RenderComponent::RenderComponent(Entity* parent, std::string modelPath, std::string texturePath, DRAW_MODE mode) : Component("Render", parent),
mode(mode), material(Vector3(0.2f, 0.2f, 0.2f), Vector3(0.4f, 0.4f, 0.4f), Vector3(0.3f, 0.3f, 0.3f), 1.0f)
{
	Loader::loadOBJ(modelPath, info);
	Loader::loadTGA(texturePath, textureID);
}


RenderComponent::RenderComponent(Entity* parent) : Component("Render", parent),
	mode(DRAW_TRIANGLES), material(Vector3(0.2f, 0.2f, 0.2f), Vector3(0.4f, 0.4f, 0.4f), Vector3(0.3f, 0.3f, 0.3f), 1.0f)
{
	Primitives::generateCube(info);
	Loader::loadTGA("Assets\\Textures\\human.tga", textureID);

}

RenderComponent::RenderComponent()
{
}


RenderComponent::~RenderComponent()
{

}

void RenderComponent::setBufferObjects(const unsigned int& VAO, const unsigned int & VBO, const unsigned int & EBO)
{
	this->VAO = VAO;
	this->VBO = VBO;
	this->EBO = EBO;
}

const OBJInfo& RenderComponent::getInfo() const
{
	return info;
}

const DRAW_MODE& RenderComponent::getMode() const
{
	return mode;
}

const Material& RenderComponent::getMaterial() const
{
	return material;
}

const unsigned int & RenderComponent::getVAO() const
{
	return VAO;
}

const unsigned int& RenderComponent::getVBO() const
{
	return VBO;
}

const unsigned int& RenderComponent::getEBO() const
{
	return EBO;
}

const unsigned int& RenderComponent::getTexID() const
{
	return textureID;
}

