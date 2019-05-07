#ifndef RENDERER_SKYBOX
#define RENDERER_SKYBOX

#include "Renderer.h"


class RendererSkybox : public Renderer
{
public:
	RendererSkybox(ShaderProgram* shader);
	RendererSkybox();
	~RendererSkybox();

	void Initialize(const BatchKey& key, Batch& batch);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);
	void Deinitialize(Batch& batch);

};

#endif