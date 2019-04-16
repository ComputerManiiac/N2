#ifndef RENDERERGRASS_H
#define RENDERERGRASS_H

#include "Renderer.h"
#include "BatchData.h"

class RendererGrass : public Renderer
{
public:

	RendererGrass(ShaderProgram* shader);
	RendererGrass();
	~RendererGrass();

	void Initialize(const BatchKey& key, Batch& batch);
	void Deinitialize(Batch& batch);
	void Update(Batch& batch, MS& modelStack);
	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);


private:
	unsigned int grassWindTexture;
	std::map<Batch*, std::vector<BatchData>> data;
};

#endif