#include "ParticleSystem.h"
#include <GL\glew.h>
#include "Primitives.h"

#include "Application.h"
#include "Manager.h"
#include "Loader.h"
#include <stdlib.h>
#include <time.h>

ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Initialize()
{
	srand((unsigned int)time(NULL));
	ShaderProgram* emitterShader = Manager::getInstance()->getShader("particle");
	emitterShader->Use();
	emitterShader->setUniform("particleTexture", 0);
	emitterShader->setUniform("projection", Manager::getInstance()->getSystem<RenderSystem>()->getProjectionMatrix());

	particleCount = 0;
	lastUsedParticle = -1;

	OBJInfo info;
	Primitives::generateQuad(info);


	glGenVertexArrays(1, &emitterVAO);
	glGenBuffers(1, &emitterVBO);
	glGenBuffers(1, &emitterEBO);

	glBindVertexArray(emitterVAO);

	glBindBuffer(GL_ARRAY_BUFFER, emitterVBO);
	glBufferData(GL_ARRAY_BUFFER, info.vertices.size() * sizeof(Vertex), &info.vertices.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, emitterEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices.at(0), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	
	glVertexAttribDivisor(0, 0);

	Loader::loadTGA("Assets\\Textures\\particle_fire.tga", textureID);
}

bool sortParticlePointers(Particle* a, Particle* b)
{
	return (*a < *b);
}

void ParticleSystem::Update(double& dt)
{

	ShaderProgram* emitterShader = Manager::getInstance()->getShader("particle");
	emitterShader->Use();

	Camera* camera = Manager::getInstance()->getCamera();
	const Mtx44& viewMatrix = camera->LookAt();
	emitterShader->setUniform("viewMatrix", viewMatrix);
	
	const Vector3& cameraPos = camera->getPos();

	modelStack.LoadIdentity();

	glDepthMask(GL_FALSE);
	for (ParticleComponent* emitter : subscribers)
	{
		std::vector<ParticleData>& particleData = data[emitter];
		std::vector<Particle*>& particles = emitterCollection[emitter];
		const float& spawnTimer = emitter->getSpawnTimer() - dt;


		/* Spawn Particle */
		if (particleCount < PARTICLE_MAX && spawnTimer <= 0.0f)
		{

			int index = findFreeParticleIndex();
			if (index == -1)
				std::cout << "[Error] No free particles although particle count is not maxed?" << std::endl;

			Particle& newParticle = particleCollection[index];
			initializeParticle(emitter, newParticle);
			particles.push_back(&newParticle);
			emitter->setSpawnTimer(emitter->getSpawnRate());
		}
		else
		{
			emitter->setSpawnTimer(spawnTimer);
		}


		if(particles.size() > 1)
			std::sort(particles.begin(), particles.end(), sortParticlePointers);

		/* Update Particle Data for Rendering */
		for(int i = 0; i < particles.size(); i++)
		{
			Particle* particle = particles[i];


			modelStack.PushMatrix();
			modelStack.Translate(particle->position);

			Mtx44 model = removeRotationFromModel(viewMatrix, modelStack.Top());

			float lifeFactor = 1 - (particle->lifeTime / emitter->getLifeTime());
			int totalCount = 8 * 8;
			float atlasProgression = lifeFactor *  totalCount;
			int index = (int) floor(atlasProgression);
			int nextIndex = index < totalCount - 1 ? index + 1 : index;
			float blend = fmod(atlasProgression, 1.0f);

			Vector2 textureCurrent;
			setTextureOffset(textureCurrent, index);
			Vector2 textureNext;
			setTextureOffset(textureNext, nextIndex);

			if (i < particleData.size())
				particleData[i].setAll(model, textureCurrent, textureNext, blend);
			else
				particleData.emplace_back(model, textureCurrent, textureNext, blend);

			modelStack.PopMatrix();


			particle->position += particle->velocity * dt;

			particle->lifeTime -= dt;
			particle->cameraDist = (cameraPos - particle->position).LengthSquared();

			/* Particle has expired */
			if (particle->lifeTime <= 0.0f)
			{
				particle->active = false;
				particles.erase(particles.begin() + i);
				--particleCount;
			}
		}


		 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glBindVertexArray(emitterVAO);
		glBindBuffer(GL_ARRAY_BUFFER, emitterBatchVBO);

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
		glVertexAttribPointer(8, 2, GL_FLOAT, false, sizeof(ParticleData), (void*) (offset + sizeof(Vector2)));
		glVertexAttribDivisor(8, 1);
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 1, GL_FLOAT, false, sizeof(ParticleData), (void*)(offset + sizeof(Vector2) + sizeof(float)));
		glVertexAttribDivisor(9, 1);



		glBufferData(GL_ARRAY_BUFFER, particleData.size() * sizeof(ParticleData), &particleData.at(0), GL_STATIC_DRAW);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, particleData.size());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glDepthMask(GL_TRUE);


}




int ParticleSystem::findFreeParticleIndex()
{

	/* Find an inactive particle from the position of the last used particle to the end of the container */
	for (int i = lastUsedParticle + 1; i < PARTICLE_MAX; i++)
	{
		if (!particleCollection[i].active)
		{
			lastUsedParticle = i;
			++particleCount;
			return i;
		}
	}

	/* Find an inactive particle from the start*/
	for (int i = 0; i < lastUsedParticle; i++)
	{
		if (!particleCollection[i].active)
		{
			lastUsedParticle = i;
			++particleCount;
			return i;
		}
	}

	/* Return -1 if there is no free particle*/
	return -1;
}


Mtx44 ParticleSystem::removeRotationFromModel(const Mtx44& viewMatrix, const Mtx44& other)
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

void ParticleSystem::initializeParticle(const ParticleComponent* emitter, Particle & particle)
{
	particle.active = true;
	particle.position = emitter->getPosition();

	if (emitter->getType() == EMITTER_SPHERE)
	{
		particle.velocity = randomPointInSphere(1.0);
	}
	else if (emitter->getType() == EMITTER_CONE)
	{
		particle.velocity = randomPointInCone(Vector3(0, 0, 0), 60.0f);
	}
	else if (emitter->getType() == EMITTER_CYLINDER)
	{
		particle.position += randomPointInCircle(0.5f);
		particle.velocity.Set(0.0f, 0.0f, 1.0f);
	}

	particle.colour = emitter->getColour();
	particle.lifeTime = emitter->getLifeTime();
}




void ParticleSystem::setTextureOffset(Vector2 & textureOffset, const int & index)
{
	float x = index % 8;
	float y = index / 8;

	textureOffset.x = (float) x / 8.0f;
	textureOffset.y = (float) (1 -  y / 8.0f);
}

float ParticleSystem::randomFloat(float a, float b)
{
	return a + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (b - a)));
}

float ParticleSystem::randomFloat(float a) {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX / a);
}

Vector3 ParticleSystem::randomPointInSphere(const float& radius) {

	Vector3 newPosition;
	float d = 0.0f;
	float rSquared = radius * radius;
	do {
		newPosition.x = randomFloat(-radius, radius);
		newPosition.y = randomFloat(-radius, radius);
		newPosition.z = randomFloat(-radius, radius);
		d = pow(newPosition.x, 2) + pow(newPosition.y, 2) + pow(newPosition.z, 2);
	} while (d > rSquared);


	return newPosition.Normalized();
}

Vector3 ParticleSystem::randomPointInCone(const Vector3& coneRotation, const float& coneWidthAngle)
{
	Vector3 newPosition;
	float halfAngle = coneWidthAngle * 0.5f;
	float randAngle = randomFloat(-halfAngle, halfAngle);

	newPosition.x = Math::DegreeToRadian(randAngle);
	newPosition.z = 1.0f;

	newPosition.Rotate(coneRotation);

	return newPosition.Normalized();
}

Vector3 ParticleSystem::randomPointInCircle(const float& radius)
{
	Vector3 newPosition;
	
	float randAngle = randomFloat(-Math::PI / 2.0f, Math::PI / 2.0f);
	float randLength = randomFloat(radius);

	newPosition.y = randLength * sin(randAngle);
	newPosition.x = randLength * cos(randAngle);

	return newPosition;
}

void ParticleSystem::registerComp(Component* component)
{
	ParticleComponent* particle = static_cast<ParticleComponent*>(component);
	if (particle == nullptr) return;
	subscribers.push_back(particle);
}

void ParticleSystem::removeComp(Component* component)
{
	ParticleComponent* particle = static_cast<ParticleComponent*>(component);
	if (particle == nullptr) return;
	subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), particle));
}

const std::vector<Particle*>& ParticleSystem::getParticlesFromEmitter(ParticleComponent* emitter)
{
	return emitterCollection[emitter];
}

void ParticleSystem::updateTexture(const Particle& particle)
{

}
