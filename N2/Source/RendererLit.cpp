#include "RendererLit.h"
#include "Manager.h"

RendererLit::RendererLit(ShaderProgram* shader) : Renderer(shader) {}

RendererLit::RendererLit()
{
}


RendererLit::~RendererLit()
{
}

void RendererLit::Initialize(const BatchKey& key, Batch& batch)
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

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);

	/* Update material values in Lit Shader */
	shader->Use();
	shader->setUniform("colorTexture", 0);
	shader->setUniform("depthTexture", 1);
	shader->setUniform("numLights", (int)LightSource::getCount());
	shader->setUniform("material.kAmbient", key.mat.ambient);
	shader->setUniform("material.kDiffuse", key.mat.diffuse);
	shader->setUniform("material.kSpecular", key.mat.specular);
	shader->setUniform("material.kShininess", key.mat.shininess);
	shader->setUniform("colorTextureEnabled", 1);
	shader->setUniform("colorTexture", 0);
	shader->setUniform("projection", projection);

}





void RendererLit::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{
	shader->Use();
	shader->setUniform("viewMatrix", Manager::getInstance()->getCamera()->LookAt());

	glBindVertexArray(batch.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, batchVBO);

	GLsizei offset = 0;
	for (unsigned int i = 3; i <= 6; ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 4, GL_FLOAT, false, sizeof(VertexData), (void*)offset);
		offset += 4 * sizeof(float);
		glVertexAttribDivisor(i, 1);
	}


	/* Buffer batch data into one large VBO */
	glBufferData(GL_ARRAY_BUFFER, batch.data.size() * sizeof(VertexData), &batch.data.at(0), GL_STATIC_DRAW);

	/* Bind current batch's texture */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	const unsigned int& indexSize = batch.info.indices.size();
	glDrawElementsInstanced(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0, batch.subscribers.size());
}


void RendererLit::Deinitialize(Batch& batch)
{
	Renderer::Deinitialize(batch);

	for (int i = 0; i <= 14; i++)
		glDisableVertexAttribArray(i);
	glDeleteVertexArrays(1, &batch.VAO);
	glDeleteBuffers(1, &batch.VBO);
	glDeleteBuffers(1, &batch.EBO);
}