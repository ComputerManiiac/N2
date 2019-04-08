#ifndef RENDERERGRASS_H
#define RENDERERGRASS_H

#include "Renderer.h"

class RendererGrass : public Renderer
{
public:

	RendererGrass(ShaderProgram* shader) : Renderer(shader) {}
	RendererGrass();
	~RendererGrass();

	void Initialize(RenderComponent* render);
	void Render(RenderComponent* render);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);


};

#endif