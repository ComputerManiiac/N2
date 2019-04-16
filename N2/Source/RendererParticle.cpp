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
	std::cout << "Called" << std::endl;
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

void RendererParticle::Update(Batch& batch, MS & modelStack)
{
	std::vector<ParticleData>& batchData = data[&batch];
	const Mtx44& viewMatrix = Manager::getInstance()->getCamera()->LookAt();

	for (Component* component : batch.subscribers) {

		ParticleComponent* emitter = static_cast<ParticleComponent*>(component);
		const std::vector<Particle*>& particles = emitter->getParticles();
		
		for (int i = 0; i < (int)particles.size(); i++) 
		{

			const Particle* particle = particles[i];

			modelStack.PushMatrix();
			modelStack.Translate(particle->position);

			Mtx44 model = removeRotationFromModel(viewMatrix, modelStack.Top());
			float lifeFactor = 1 - (particle->lifeTime / emitter->getLifeTime());
			int totalCount = 8 * 8;
			float atlasProgression = lifeFactor * totalCount;
			int index = (int)floor(atlasProgression);
			int nextIndex = index < totalCount - 1 ? index + 1 : index;
			float blend = fmod(atlasProgression, 1.0f);

			Vector2 textureCurrent;
			setTextureOffset(textureCurrent, index);
			Vector2 textureNext;
			setTextureOffset(textureNext, nextIndex);

			/* Update MVP */
			if (i < batchData.size())
				batchData[i].setAll(model, textureCurrent, textureNext, blend);
			else
				batchData.emplace_back(model, textureCurrent, textureNext, blend);

			modelStack.PopMatrix();
		}
	}
}

void RendererParticle::Render(Batch& batch, const unsigned int& textureID, MS& modelStack, const Mtx44& view)
{
	glDepthMask(GL_TRUE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	std::vector<ParticleData>& batchData = data[&batch];

	if (batchData.size() > 0) {
		glBufferData(GL_ARRAY_BUFFER, batchData.size() * sizeof(ParticleData), &batchData.at(0), GL_STATIC_DRAW);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, batchData.size());
	}

	GLsizei offset = 0;
	for (unsigned int i = 3; i <= 6; ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 4, GL_FLOAT, false, sizeof(ParticleData), (void*)offset);
		offset += 4 * sizeof(float);
		glVertexAttribDivisor(i, 1);
	}

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 2, GL_FLOAT, false, sizeof(ParticleData), (void*)offset);
	glVertexAttribDivisor(7, 1);
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 2, GL_FLOAT, false, sizeof(ParticleData), (void*)(offset + sizeof(Vector2)));
	glVertexAttribDivisor(8, 1);
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 1, GL_FLOAT, false, sizeof(ParticleData), (void*)(offset + sizeof(Vector2) + sizeof(float)));
	glVertexAttribDivisor(9, 1);

	shader->Use();

	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDepthMask(GL_TRUE);
	
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