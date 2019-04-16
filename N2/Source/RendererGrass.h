#ifndef RENDERERGRASS_H
#define RENDERERGRASS_H

#include "Renderer.h"

class RendererGrass : public Renderer
{
public:

	RendererGrass(ShaderProgram* shader);
	RendererGrass();
	~RendererGrass();

	void Initialize(const BatchKey& key, Batch& batch);
	void Initialize(const std::vector<LightSource*>& lightSources);
	void Deinitialize(Batch& batch);
	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);

private:
	unsigned int grassWindTexture;
};

#endif