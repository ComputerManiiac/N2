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



void RendererShadow::Update(Batch& batch, MS& modelStack)
{
	std::vector<Mtx44>& batchData = data[&batch];

	for (int i = 0; i < (int)batch.subscribers.size(); i++)
	{
		RenderComponent* sub = static_cast<RenderComponent*>(batch.subscribers[i]);
		modelStack.PushMatrix();
		TransformComponent* transform = sub->getParent()->getComponent<TransformComponent>();
		modelStack.Translate(transform->getPos());
		modelStack.Rotate(transform->getRot());
		modelStack.Scale(transform->getScale());

		/* Update MVP */
		if (i < batchData.size())
			batchData[i] = modelStack.Top();
		else
			batchData.emplace_back(modelStack.Top());

		modelStack.PopMatrix();
	}
}

void RendererShadow::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{
	const std::vector<Mtx44>& modelMatrices = data[&batch];

	glBindVertexArray(batch.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);

	GLsizei offset = 0;
	for (unsigned int i = 3; i <= 6; ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 4, GL_FLOAT, false, sizeof(Mtx44), (void*)offset);
		offset += 4 * sizeof(float);
		glVertexAttribDivisor(i, 1);
	}

	/* Buffer batch data into one large VBO */
	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(Mtx44), &modelMatrices.at(0), GL_STATIC_DRAW);

	shader->Use();
	const unsigned int& indexSize = batch.info.indices.size();
	glDrawElementsInstanced(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0, batch.subscribers.size());
}

