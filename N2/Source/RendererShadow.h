#ifndef RENDERERSHADOW_H
#define RENDERERSHADOW_H

#include "Renderer.h"

class RendererShadow : public Renderer
{
public:

	RendererShadow(ShaderProgram* shader);
	RendererShadow();
	~RendererShadow();

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);

	void setModelMatricesForBatch(Batch* batch, const std::vector<Mtx44>& modelMatrices);

private:
	std::map<Batch*, std::vector<Mtx44>> data;
};

#endif