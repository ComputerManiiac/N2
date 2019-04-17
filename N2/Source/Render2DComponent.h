#ifndef RENDER2DCOMPONENT_H
#define RENDER2DCOMPONENT_H

#include "Component.h"
#include "ShaderProgram.h"

class Render2DComponent : public Component
{
public:
	Render2DComponent(Entity* parent, ShaderProgram* shader, std::string texturePath);
	Render2DComponent();
	~Render2DComponent();

	const unsigned int& getTexID() const;
	ShaderProgram* getShader() const;

private:
	unsigned int textureID;
	ShaderProgram* shader;
};
#endif