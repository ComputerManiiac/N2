#include "RendererGrass.h"
#include "Manager.h"
#include "Loader.h"
#include "Application.h"


RendererGrass::RendererGrass(ShaderProgram* shader) : Renderer(shader)
{
	shader->Use();
	shader->setUniform("colorTexture", 0);
	shader->setUniform("depthTexture", 1);
	shader->setUniform("windTexture", 2);
	shader->setUniform("windStrength", 0.1f);
	shader->setUniform("windSpeed", 0.5f);
	Loader::loadTGA("Assets\\Textures\\grasswind.tga", grassWindTexture);
}

RendererGrass::RendererGrass()
{

}


RendererGrass::~RendererGrass()
{
}

void RendererGrass::Initialize(const std::vector<LightSource*>& lightSources)
{


	
}

void RendererGrass::Initialize(RenderComponent* sub)
{


	const OBJInfo& info = sub->getInfo();

	/* Buffer data into VBO and EBO*/
	glBindVertexArray(sub->getVAO());

	glBindBuffer(GL_ARRAY_BUFFER, sub->getVBO());
	glBufferData(GL_ARRAY_BUFFER, info.vertices.size() * sizeof(Vertex), &info.vertices.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub->getEBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices.at(0), GL_STATIC_DRAW);

	/* Indicate layout of data passed in */
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Vector3));
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(Vector3)));

	/* Update material values in Lit Shader */
	const Material& mat = sub->getMaterial();
	shader->Use();
	shader->setUniform("material.kAmbient", mat.ambient);
	shader->setUniform("material.kDiffuse", mat.diffuse);
	shader->setUniform("material.kSpecular", mat.specular);
	shader->setUniform("material.kShininess", mat.shininess);
	shader->setUniform("colorTextureEnabled", 1);
	shader->setUniform("colorTexture", 0);
}

void RendererGrass::Render(RenderComponent* sub)
{

}

void RendererGrass::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{
	shader->Use();
	shader->setUniform("timeElapsed", (float)glfwGetTime());

	batch.data.clear();
	batch.data.reserve(batch.subscribers.size());

	glBindBuffer(GL_ARRAY_BUFFER, batchVBO);

	for (RenderComponent* sub : batch.subscribers)
	{
		/* Layout instanced VBO data */
		glBindVertexArray(sub->getVAO());
		GLsizei offset = 0;
		for (unsigned int i = 3; i <= 14; ++i)
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
		batch.data.emplace_back(modelStack.Top(), view, projection);
		modelStack.PopMatrix();
	}

	/* Buffer batch data into one large VBO */
	glBufferData(GL_ARRAY_BUFFER, batch.data.size() * sizeof(VertexData), &batch.data.at(0), GL_STATIC_DRAW);

	/* Bind current batch's texture */
	shader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, grassWindTexture);

	glDisable(GL_CULL_FACE);

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

	glEnable(GL_CULL_FACE);
}