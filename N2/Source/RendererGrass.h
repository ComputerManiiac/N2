#ifndef RENDERERGRASS_H
#define RENDERERGRASS_H

#include "Renderer.h"

class RendererGrass : public Renderer
{
public:

	RendererGrass(ShaderProgram* shader);
	RendererGrass();
	~RendererGrass();

	void Initialize(const std::vector<LightSource*>& lightSources);
	void Initialize(RenderComponent* render);
	void Render(RenderComponent* render);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);

private:
	unsigned int grassWindTexture;
};

#endif