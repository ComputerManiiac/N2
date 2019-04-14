#ifndef RENDERERPARTICLE_H
#define RENDERERPARTICLE_H

#include "Renderer.h"

class RendererParticle : public Renderer
{
public:

	RendererParticle(ShaderProgram* shader);
	RendererParticle();
	~RendererParticle();

	void Initialize();
	void Initialize(const BatchKey& key, Batch& batch);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);
	void Deinitialize(Batch& batch);
};

#endif