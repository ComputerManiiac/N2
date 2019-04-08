#ifndef RENDERERLIT_H
#define RENDERERLIT_H

#include "Renderer.h"

class RendererLit : public Renderer
{
public:

	RendererLit(ShaderProgram* shader) : Renderer(shader) {}
	RendererLit();
	~RendererLit();

	void Initialize(RenderComponent* render);
	void Render(RenderComponent* render);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);

};

#endif