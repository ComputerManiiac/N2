#ifndef RENDERERLIT_H
#define RENDERERLIT_H

#include "Renderer.h"
#include "LightSource.h"

class RendererLit : public Renderer
{
public:

	RendererLit(ShaderProgram* shader) : Renderer(shader) {}
	RendererLit();
	~RendererLit();

	void Initialize(const BatchKey& key, Batch& batch);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);
	void Deinitialize(Batch& batch);

};

#endif