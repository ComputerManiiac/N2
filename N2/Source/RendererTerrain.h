#ifndef RENDERERTERRAIN_H
#define RENDERERTERRAIN_H

#include "Renderer.h"
#include "LightSource.h"

class RendererTerrain : public Renderer
{
public:

	RendererTerrain(ShaderProgram* shader);
	RendererTerrain();
	~RendererTerrain();

	void Initialize(const BatchKey& key, Batch& batch);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);
	void Deinitialize(Batch& batch);



};

#endif