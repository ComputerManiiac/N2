#ifndef RENDERERLIT_H
#define RENDERERLIT_H

#include "Renderer.h"

class RendererLit : public Renderer
{
public:

	RendererLit(ShaderProgram* shader);
	RendererLit();
	~RendererLit();

	void Initialize(const BatchKey& key, Batch& batch);
	void Update(Batch& batch, MS& modelStack);
	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);
	void Deinitialize(Batch& batch);

private:
	std::map<Batch*, std::vector<BatchData>> data;

};

#endif