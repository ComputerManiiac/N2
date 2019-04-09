#include "RendererShadow.h"
#include "Manager.h"



RendererShadow::RendererShadow(ShaderProgram* shader) : Renderer(shader)
{

}

RendererShadow::RendererShadow()
{
}


RendererShadow::~RendererShadow()
{
}

void RendererShadow::Initialize(Batch& batch) {

	Renderer::Initialize(batch);

	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
	//glBufferData(GL_ARRAY_BUFFER, batch.info.vertices.size() * sizeof(Vector3), &batch.info.vertices.at(0), GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelEBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch.info.indices.size() * sizeof(unsigned int), &batch.info.indices.at(0), GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vector3), 0);
	//glVertexAttribDivisor(0, 0);
}

void RendererShadow::Initialize(RenderComponent* sub)
{
	
}

void RendererShadow::Render(RenderComponent* sub)
{

}

void RendererShadow::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{
	std::cout << "Render Shadow -> Render " << std::endl;
	modelMatrices.clear();
	modelMatrices.reserve(batch.subscribers.size());

	glBindVertexArray(batch.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, batchVBO);

	glEnableVertexAttribArray(1);
	GLsizei offset = 0;
	for (unsigned int i = 1; i <= 4; ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 4, GL_FLOAT, false, sizeof(Mtx44), (void*)offset);
		offset += 4 * sizeof(float);
		glVertexAttribDivisor(i, 1);
	}

	for (RenderComponent* sub : batch.subscribers) {

		if (!sub->isActive()) continue;
		modelStack.PushMatrix();
		TransformComponent* transform = sub->getParent()->getComponent<TransformComponent>();
		modelStack.Translate(transform->getPos());
		modelStack.Rotate(transform->getRot());
		modelStack.Scale(transform->getScale());
		modelMatrices.push_back(modelStack.Top());
		modelStack.PopMatrix();
	}

	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(Mtx44), &modelMatrices.at(0), GL_STATIC_DRAW);

	shader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawElementsInstanced(GL_TRIANGLES, batch.info.indices.size(), GL_UNSIGNED_INT, 0, batch.subscribers.size());
}