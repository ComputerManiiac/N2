#include "RendererParticle.h"
#include "Manager.h"
#include "Loader.h"
#include "Primitives.h"

RendererParticle::RendererParticle(ShaderProgram* shader) : Renderer(shader) {}


RendererParticle::RendererParticle()
{
}


RendererParticle::~RendererParticle()
{
}

void RendererParticle::Initialize()
{
	srand((unsigned int)time(NULL));

	ShaderProgram* emitterShader = Manager::getInstance()->getShader("particle");
	emitterShader->Use();
	emitterShader->setUniform("particleTexture", 0);
	emitterShader->setUniform("projection", Manager::getInstance()->getSystem<RenderSystem>()->getProjectionMatrix());


	OBJInfo info;
	Primitives::generateQuad(info);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, info.vertices.size() * sizeof(Vertex), &info.vertices.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices.at(0), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);

	glVertexAttribDivisor(0, 0);

}

void RendererParticle::Initialize(const BatchKey& key, Batch& batch)
{
}

void RendererParticle::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{


}

void RendererParticle::Deinitialize(Batch& batch)
{
}



Mtx44 RendererParticle::removeRotationFromModel(const Mtx44& viewMatrix, const Mtx44& other)
{
	Mtx44 billboard = other;
	billboard.a[0] = viewMatrix.a[0];
	billboard.a[1] = viewMatrix.a[4];
	billboard.a[2] = viewMatrix.a[8];

	billboard.a[4] = viewMatrix.a[1];
	billboard.a[5] = viewMatrix.a[5];
	billboard.a[6] = viewMatrix.a[9];

	billboard.a[8] = viewMatrix.a[2];
	billboard.a[9] = viewMatrix.a[6];
	billboard.a[10] = viewMatrix.a[10];
	return billboard;
}

void RendererParticle::setTextureOffset(Vector2& textureOffset, const int& index)
{
	float x = index % 8;
	float y = index / 8;

	textureOffset.x = (float)x / 8.0f;
	textureOffset.y = (float)(1 - y / 8.0f);
}