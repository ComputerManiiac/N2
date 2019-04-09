#include "RendererLit.h"
#include "Manager.h"


RendererLit::RendererLit()
{
}


RendererLit::~RendererLit()
{
}

void RendererLit::Initialize(Batch& batch) {

	Renderer::Initialize(batch);


	shader->Use();
	shader->setUniform("colorTexture", 0);
	shader->setUniform("depthTexture", 1);
	shader->setUniform("numLights", (int)LightSource::getCount());


	glBindVertexArray(batch.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, batch.VBO);
	glBufferData(GL_ARRAY_BUFFER, batch.info.vertices.size() * sizeof(VertexData), &(batch.info.vertices.at(0)), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch.info.indices.size() * sizeof(unsigned int), &(batch.info.indices.at(0)), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexData), (void*) sizeof(Vector3));
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(VertexData), (void*)(sizeof(Vector3) + sizeof(Vector3)));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);
}

void RendererLit::Initialize(const std::vector<LightSource*>& lightSources)
{


}


void RendererLit::Initialize(RenderComponent* sub)
{
	//const OBJInfo& info = sub->getInfo();

	///* Buffer data into VBO and EBO*/
	//glBindVertexArray(sub->getVAO());

	//glBindBuffer(GL_ARRAY_BUFFER, sub->getVBO());
	//glBufferData(GL_ARRAY_BUFFER, info.vertices.size() * sizeof(Vertex), &info.vertices.at(0), GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub->getEBO());
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices.at(0), GL_STATIC_DRAW);

	///* Indicate layout of data passed in */
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	//glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Vector3));
	//glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(Vector3)));

	///* Update material values in Lit Shader */
	//const Material& mat = sub->getMaterial();
	//shader->Use();
	//shader->setUniform("material.kAmbient", mat.ambient);
	//shader->setUniform("material.kDiffuse", mat.diffuse);
	//shader->setUniform("material.kSpecular", mat.specular);
	//shader->setUniform("material.kShininess", mat.shininess);
	//shader->setUniform("colorTextureEnabled", 1);
	//shader->setUniform("colorTexture", 0);
}

void RendererLit::Render(RenderComponent* sub) {}

void RendererLit::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{
	std::cout << "RenderLit -> Render " << std::endl;
	batch.data.clear();
	batch.data.reserve(batch.subscribers.size());

	glBindVertexArray(batch.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, batchVBO);

	GLsizei offset = 0;
	for (unsigned int i = 3; i <= 14; ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 4, GL_FLOAT, false, sizeof(VertexData), (void*)offset);
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
		batch.data.emplace_back(modelStack.Top(), view, projection);
		modelStack.PopMatrix();
	}

	glBufferData(GL_ARRAY_BUFFER, batch.data.size() * sizeof(VertexData), &batch.data.at(0), GL_STATIC_DRAW);

	shader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawElementsInstanced(GL_TRIANGLES, batch.info.indices.size(), GL_UNSIGNED_INT, 0, batch.subscribers.size());

}