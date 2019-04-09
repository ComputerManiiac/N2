#ifndef RENDERER_H
#define RENDERER_H
	
#include "RenderComponent.h"
#include "Mtx44.h"
#include "MatrixStack.h"
#include "Batch.h"
#include "Entity.h"
#include "LightSource.h"

class Renderer
{
public:
	Renderer(ShaderProgram* shader, bool isBatched=true);
	Renderer();
	~Renderer();

	virtual void Initialize(const std::vector<LightSource*>& lightSources);
	virtual void Initialize(RenderComponent* render);
	virtual void Render(RenderComponent* render);
	virtual void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);

protected:
	ShaderProgram* shader;
	unsigned int batchVBO;
	bool isBatched;
	Mtx44 projection;
};

#endif