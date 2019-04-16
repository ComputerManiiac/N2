#include "RendererGrass.h"
#include "Manager.h"
#include "Loader.h"
#include "Application.h"


RendererGrass::RendererGrass(ShaderProgram* shader) : Renderer(shader)
{

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

void RendererGrass::Deinitialize(Batch & batch)
{

	Renderer::Deinitialize(batch);

	for (int i = 0; i <= 14; i++)
		glDisableVertexAttribArray(i);
	glDeleteVertexArrays(1, &batch.VAO);
	glDeleteBuffers(1, &batch.VBO);
	glDeleteBuffers(1, &batch.EBO);
}



void RendererGrass::Initialize(const BatchKey& key, Batch& batch)
{
	Renderer::Initialize(key, batch);

	/* Buffer data into VBO and EBO*/
	glBindVertexArray(batch.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, batch.VBO);
	glBufferData(GL_ARRAY_BUFFER, batch.info.vertices.size() * sizeof(Vertex), &batch.info.vertices.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch.info.indices.size() * sizeof(unsigned int), &batch.info.indices.at(0), GL_STATIC_DRAW);

	/* Indicate layout of data passed in */
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Vector3));
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(Vector3)));

	/* Update material values in Lit Shader */
	shader->Use();
	shader->setUniform("colorTexture", 0);
	shader->setUniform("depthTexture", 1);
	shader->setUniform("windTexture", 2);
	shader->setUniform("windStrength", 0.1f);
	shader->setUniform("windSpeed", 0.5f);
	shader->setUniform("numLights", (int)LightSource::getCount());

	if (batch.subscribers.size() > 0) {
		const Material& mat = static_cast<RenderComponent*>(batch.subscribers.at(0))->getMaterial();
		shader->setUniform("material.kAmbient", mat.ambient);
		shader->setUniform("material.kDiffuse", mat.diffuse);
		shader->setUniform("material.kSpecular", mat.specular);
		shader->setUniform("material.kShininess", mat.shininess);
	}
	shader->setUniform("colorTextureEnabled", 1);
	shader->setUniform("projection", projection);
}

void RendererGrass::Update(Batch& batch, MS& modelStack)
{
	std::vector<BatchData>& batchData = data[&batch];

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

void RendererGrass::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{

	std::vector<BatchData>& batchData = data[&batch];
	shader->Use();
	shader->setUniform("timeElapsed", (float)glfwGetTime());
	shader->setUniform("viewMatrix", Manager::getInstance()->getCamera()->LookAt());

	glDepthFunc(GL_LESS);

	glBindVertexArray(batch.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);

	GLsizei offset = 0;
	for (unsigned int i = 3; i <= 6; ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 4, GL_FLOAT, false, sizeof(VertexData), (void*)offset);
		offset += 4 * sizeof(float);
		glVertexAttribDivisor(i, 1);
	}


	/* Buffer batch data into one large VBO */
	glBufferData(GL_ARRAY_BUFFER, batchData.size() * sizeof(VertexData), &batchData.at(0), GL_STATIC_DRAW);

	/* Bind current batch's texture */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, grassWindTexture);

	glDisable(GL_CULL_FACE);
	const unsigned int& indexSize = batch.info.indices.size();
	glDrawElementsInstanced(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0, batch.subscribers.size());
	glEnable(GL_CULL_FACE);


}
