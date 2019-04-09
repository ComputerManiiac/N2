#include "Renderer.h"
#include "Manager.h"

Renderer::Renderer(ShaderProgram* shader, bool isBatched) : shader(shader), isBatched(isBatched)
{

}

Renderer::Renderer() {}

Renderer::~Renderer()
{
	if(!isBatched)
		glDeleteBuffers(1, &batchVBO);
}

void Renderer::Initialize(Batch& batch) {
	projection = Manager::getInstance()->getSystem<RenderSystem>()->getProjectionMatrix();

	if (isBatched)
		glGenBuffers(1, &batchVBO);

	glGenVertexArrays(1, &batch.VAO);
	glGenBuffers(1, &batch.VBO);
	glGenBuffers(1, &batch.EBO);

}

void Renderer::Initialize(const std::vector<LightSource*>& lightSources)
{
}

void Renderer::Initialize(RenderComponent* render)
{

}

void Renderer::Render(RenderComponent* render)
{

}

void Renderer::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{
	

}