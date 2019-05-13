#ifndef RENDERERWATER_H
#define RENDERERWATER_H

#include "Renderer.h"

class RendererWater : public Renderer
{
public:

	RendererWater(ShaderProgram* shader);
	RendererWater();
	~RendererWater();

	void Initialize(const BatchKey& key, Batch& batch);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);
	void Deinitialize(Batch& batch);

};

#endif