#ifndef RENDER2DCOMPONENT_H
#define RENDER2DCOMPONENT_H

#include "Component.h"
#include "ShaderProgram.h"

class Render2DComponent : public Component
{
public:
	Render2DComponent(Entity* parent, ShaderProgram* shader, std::string texturePath, Vector2 textureOffset, Vector2 textureSize);
	Render2DComponent();
	~Render2DComponent();

	void setTextureOffset(const Vector2& textureOffset);
	void setTextureSize(const Vector2& textureSize);

	const unsigned int& getTexID() const;
	const Vector2& getTextureSize() const;
	const Vector2& getTextureOffset() const;
	ShaderProgram* getShader() const;
	

private:
	unsigned int textureID;
	Vector2 textureOffset;
	Vector2 textureSize;
	ShaderProgram* shader;
};
#endif