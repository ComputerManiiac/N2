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


void RendererShadow::Initialize(RenderComponent* sub)
{
	
}

void RendererShadow::Render(RenderComponent* sub)
{

}

void RendererShadow::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{
	modelMatrices.clear();
	modelMatrices.reserve(batch.subscribers.size());

	glBindBuffer(GL_ARRAY_BUFFER, batchVBO);

	for (RenderComponent* sub : batch.subscribers)
	{
		/* Layout instanced VBO data */
		glBindVertexArray(sub->getVAO());
		GLsizei offset = 0;
		for (unsigned int i = 3; i <= 6; ++i)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, 4, GL_FLOAT, false, sizeof(VertexData), (void*)offset);
			offset += 4 * sizeof(float);
			glVertexAttribDivisor(i, 1);
		}

		/* Populate batch data with current component's data */
		modelStack.PushMatrix();
		TransformComponent* transform = sub->getParent()->getComponent<TransformComponent>();
		modelStack.Translate(transform->getPos());
		modelStack.Rotate(transform->getRot());
		modelStack.Scale(transform->getScale());
		modelMatrices.push_back(modelStack.Top());
		modelStack.PopMatrix();
	}

	/* Buffer batch data into one large VBO */
	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(Mtx44), &modelMatrices.at(0), GL_STATIC_DRAW);

	/* Bind current batch's texture */
	shader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	/* Render each component within the batch */
	for (RenderComponent* sub : batch.subscribers)
	{
		if (!sub->isActive()) continue;

		glBindVertexArray(sub->getVAO());

		const unsigned int& indexSize = sub->getInfo().indices.size();
		const DRAW_MODE& mode = sub->getMode();

		if (mode == DRAW_TRIANGLE_STRIP)
			glDrawElementsInstanced(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, 0, batch.subscribers.size());
		else if (mode == DRAW_LINES)
			glDrawElementsInstanced(GL_LINES, indexSize, GL_UNSIGNED_INT, 0, batch.subscribers.size());
		else if (mode == DRAW_FAN)
			glDrawElementsInstanced(GL_TRIANGLE_FAN, indexSize, GL_UNSIGNED_INT, 0, batch.subscribers.size());
		else
			glDrawElementsInstanced(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0, batch.subscribers.size());

	}
}