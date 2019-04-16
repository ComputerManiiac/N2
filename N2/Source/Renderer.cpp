#include "Renderer.h"
#include "Manager.h"

Renderer::Renderer(ShaderProgram* shader, bool isBatched) : shader(shader), isBatched(isBatched)
{
	projection = Manager::getInstance()->getSystem<RenderSystem>()->getProjectionMatrix();

	if (isBatched)
		glGenBuffers(1, &instancedVBO);
}

Renderer::Renderer() {}

Renderer::~Renderer()
{

}

void Renderer::Initialize(const BatchKey& key, Batch& batch)
{

	glGenVertexArrays(1, &batch.VAO);
	glGenBuffers(1, &batch.VBO);
	glGenBuffers(1, &batch.EBO);
}

void Renderer::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{

}

void Renderer::Deinitialize(Batch& batch)
{
	if (isBatched)
		glDeleteBuffers(1, &instancedVBO);
}