#ifndef RENDER2DCOMPONENT_H
#define RENDER2DCOMPONENT_H

#include "Component.h"
#include "ShaderProgram.h"

class Render2DComponent : public Component
{
public:
	Render2DComponent(Entity* parent, ShaderProgram* shader, std::string texturePath, Vector2 textureOffset);
	Render2DComponent();
	~Render2DComponent();

	const unsigned int& getTexID() const;
	const Vector2& getTextureOffset() const;
	ShaderProgram* getShader() const;
	

private:
	unsigned int textureID;
	Vector2 textureOffset;
	ShaderProgram* shader;
};
#endif