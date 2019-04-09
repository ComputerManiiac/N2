#ifndef RENDERERSKYBOX_H
#define RENDERERSKYBOX_H

#include "Renderer.h"

class RendererSkybox : public Renderer
{
public:
	RendererSkybox(ShaderProgram* shader);
	RendererSkybox();
	~RendererSkybox();

	void Initialize();
	void Render(MS& modelStack);

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int textureID;
};

#endif