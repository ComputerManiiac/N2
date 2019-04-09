#ifndef RENDERERSHADOW_H
#define RENDERERSHADOW_H

#include "Renderer.h"

class RendererShadow : public Renderer
{
public:

	RendererShadow(ShaderProgram* shader);
	RendererShadow();
	~RendererShadow();

	void Initialize(Batch& batch);
	void Initialize(RenderComponent* render);
	void Render(RenderComponent* render);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);

private:
	std::vector<Mtx44> modelMatrices;
};

#endif