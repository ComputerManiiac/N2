#ifndef RENDERERPARTICLE_H
#define RENDERERPARTICLE_H

#include "Renderer.h"

class RendererParticle : public Renderer
{
public:

	RendererParticle(ShaderProgram* shader);
	RendererParticle();
	~RendererParticle();

	void Initialize();
	void Initialize(const BatchKey& key, Batch& batch);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);
	void Deinitialize(Batch& batch);

private:

	Mtx44 removeRotationFromModel(const Mtx44& viewMatrix, const Mtx44& other);
	void setTextureOffset(Vector2& textureOffset, const int& index);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

#endif