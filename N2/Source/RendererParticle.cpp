#include "RendererParticle.h"


RendererParticle::RendererParticle(ShaderProgram* shader) : Renderer(shader) {}


RendererParticle::RendererParticle()
{
}


RendererParticle::~RendererParticle()
{
}

void RendererParticle::Initialize()
{

}

void RendererParticle::Initialize(const BatchKey & key, Batch & batch)
{
}

void RendererParticle::Render(Batch& batch, const unsigned int & textureID, MS & modelStack, const Mtx44 & view)
{
}

void RendererParticle::Deinitialize(Batch& batch)
{
}


