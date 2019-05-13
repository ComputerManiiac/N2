#ifndef RENDERERWATER_H
#define RENDERERWATER_H

#include "Renderer.h"

class RendererWater : public Renderer
{
public:

	RendererWater(ShaderProgram* shader);
	RendererWater();
	~RendererWater();

	void Initialize(const BatchKey& key, Batch& batch);

	void Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view);
	void Deinitialize(Batch& batch);

	void bindReflectionFBO();
	void bindRefractionFBO();

	const unsigned int& getReflectionID() const;
	const unsigned int& getRefractionID() const;

private:

	void generateFrameBuffer(unsigned int& FBO, unsigned int& RBO, unsigned int& textureID);

	unsigned int reflectionTexID;
	unsigned int refractionTexID;
	unsigned int reflectionFBO;
	unsigned int refractionFBO;
	unsigned int reflectionRBO;
	unsigned int refractionRBO;
};

#endif