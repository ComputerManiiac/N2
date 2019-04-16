#ifndef RENDERERSHADOW_H
#define RENDERERSHADOW_H

#include "Renderer.h"

class RendererShadow : public Renderer
{
public:

	RendererShadow(ShaderProgram* shader);
	RendererShadow();
	~RendererShadow();

	void Update(Batch& batch, MS& modelStack);
	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);

	

private:
	std::map<Batch*, std::vector<Mtx44>> data;
};

#endif