#include "RenderSystem.h"
#include "Application.h"
#include "Manager.h"
#include "Entity.h"
#include "Utility.h"
#include "Primitives.h"


RenderSystem::RenderSystem()
{

}



RenderSystem::~RenderSystem()
{
	glDeleteBuffers(1, &matricesVBO);

	for (RenderComponent* sub : subscribers)
	{
		glDeleteVertexArrays(1, &sub->getVAO());
		glDeleteBuffers(1, &sub->getVBO());
		glDeleteBuffers(1, &sub->getEBO());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(7);
		glDisableVertexAttribArray(11);

		ColliderComponent* collider = sub->getParent()->getComponent<ColliderComponent>();
		if (collider != nullptr)
		{
			glDeleteVertexArrays(1, &collider->getVAO());
			glDeleteBuffers(1, &collider->getVBO());
			glDeleteBuffers(1, &collider->getEBO());

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(3);
			glDisableVertexAttribArray(7);
			glDisableVertexAttribArray(11);
		}
	}


	for (LightSource* light : lightSources)
	{
		delete light;
	}
}

void RenderSystem::Initialize() {

	lit = Manager::getInstance()->getShader("lit");
	LightSource::setShader(lit);
	lit->Use();

	// Defines projection matrix for the scene
	Mtx44 proj;
	proj.SetToPerspective(45.0f, (float)Application::getScreenWidth() / (float)Application::getScreenHeight(), 0.1f, 10000.0f);
	projectionStack.LoadMatrix(proj);

	glGenBuffers(1, &matricesVBO);

	// Generates vertex arrays and buffers for each render component and populates the assigned int into them
	for (RenderComponent* sub : subscribers)
	{
		unsigned int VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		initializeComponent(sub->getInfo(), VAO, VBO, EBO);
		sub->setBufferObjects(VAO, VBO, EBO);

		const Material& mat = sub->getMaterial();

		lit->setUniform("material.kAmbient", mat.ambient);
		lit->setUniform("material.kDiffuse", mat.diffuse);
		lit->setUniform("material.kSpecular", mat.specular);
		lit->setUniform("material.kShininess", mat.shininess);
		lit->setUniform("colorTextureEnabled", 1);
		lit->setUniform("colorTexture", 0);

		ColliderComponent* collider = sub->getParent()->getComponent<ColliderComponent>();
		if (collider != nullptr)
		{
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			initializeComponent(collider->getInfo(), VAO, VBO, EBO);
			collider->setBufferObjects(VAO, VBO, EBO);
		}
	}

	setupLight();
}

/* Sets up how attributes are layed out in the VAO before buffering data into VBO */
void RenderSystem::initializeComponent(const OBJInfo& info, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, info.vertices.size() * sizeof(Vertex), &info.vertices.at(0), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices.at(0), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Vector3));
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(Vector3)));
}

/* Sets up values for all the lights in the Scene */ 
void RenderSystem::setupLight()
{
	LightSource* light = new LightSource(LIGHT_POINT);
	light->setPointLight(lit, Vector3(0, 5, 0), Vector3(0, 0, 1), 1.0f, 1.0f, 0.01f, 0.001f);
	lightSources.push_back(light);

	LightSource* light2 = new LightSource(LIGHT_POINT);
	light2->setPointLight(lit, Vector3(5, 5, 0), Vector3(1, 0, 0), 1.0f, 1.0f, 0.01f, 0.001f);
	lightSources.push_back(light2);

	LightSource* light3 = new LightSource(LIGHT_SPOTLIGHT);
	light3->setSpotLight(lit, Vector3(0, 4, 0), Vector3(1, 1, 1), Vector3(0, -1, 0), 1.0f, 45.0f, 30.0f, 1.0f, 1.0f, 0.01f, 0.001f);
	lightSources.push_back(light3);

	lit->setUniform("numLights", (int) LightSource::getCount());
}


// Uses data that are stored in the RenderComponent (stored previously through Initialize) to draw objects
void RenderSystem::Update(double& dt)
{
	//PerformanceClock clock;
	modelStack.LoadIdentity();
	viewStack.LoadMatrix(Manager::getInstance()->getCamera()->LookAt());

	updateTransformMatrices();

	for (RenderComponent* sub : subscribers)
	{
		if (!sub->isActive()) continue;

		glBindVertexArray(sub->getVAO());

		const unsigned int& indexSize = sub->getInfo().indices.size();
		const DRAW_MODE& mode = sub->getMode();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sub->getTexID());

		if (mode == DRAW_TRIANGLE_STRIP)
			glDrawElementsInstanced(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, 0, subscribers.size());
		else if (mode == DRAW_LINES)
			glDrawElementsInstanced(GL_LINES, indexSize, GL_UNSIGNED_INT, 0, subscribers.size());
		else if (mode == DRAW_FAN)
			glDrawElementsInstanced(GL_TRIANGLE_FAN, indexSize, GL_UNSIGNED_INT, 0, subscribers.size());
		else
			glDrawElementsInstanced(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0, subscribers.size());
		

		/* Drawing colliders can be costly - DEBUG ONLY! */
		ColliderComponent* collider = sub->getParent()->getComponent<ColliderComponent>();
		if (collider != nullptr && collider->shouldDrawCollider())
		{
			glBindVertexArray(collider->getVAO());
			lit->setUniform("colorTextureEnabled", 0);
			lit->setUniform("lightEnabled", false);
			glDrawElementsInstanced(GL_LINES, indexSize, GL_UNSIGNED_INT, 0, subscribers.size());
			lit->setUniform("lightEnabled", true);
			lit->setUniform("colorTextureEnabled", 1);
		}
	}
}

void RenderSystem::layoutInstancedData(const unsigned int& VAO)
{
	glBindVertexArray(VAO);
	GLsizei offset = 0;
	for (unsigned int i = 3; i <= 14; ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 4, GL_FLOAT, false, sizeof(VertexData), (void*)offset);
		offset += 4 * sizeof(float);
		glVertexAttribDivisor(i, 1);
	}
}

void RenderSystem::updateTransformMatrices()
{
	data.clear();
	data.reserve(subscribers.size());

	glBindBuffer(GL_ARRAY_BUFFER, matricesVBO);

	for (RenderComponent* sub : subscribers)
	{
		modelStack.PushMatrix();

		layoutInstancedData(sub->getVAO());

		TransformComponent* transform = sub->getParent()->getComponent<TransformComponent>();
		modelStack.Translate(transform->getPos());
		modelStack.Rotate(transform->getRot());
		modelStack.Scale(transform->getScale());
		data.emplace_back(modelStack.Top(), viewStack.Top(), projectionStack.Top());

		/* Drawing colliders can be costly - DEBUG ONLY! */
		ColliderComponent* collider = sub->getParent()->getComponent<ColliderComponent>();
		if (collider != nullptr && collider->shouldDrawCollider())
		{
			layoutInstancedData(collider->getVAO());
			modelStack.PushMatrix();
			modelStack.Scale(collider->getScale());
			modelStack.PopMatrix();
			data.emplace_back(modelStack.Top(), viewStack.Top(), projectionStack.Top());
		}
		modelStack.PopMatrix();
	}

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(VertexData), &data.at(0), GL_STATIC_DRAW);

}





void RenderSystem::registerComp(Component* component)
{
	RenderComponent* render = dynamic_cast<RenderComponent*>(component);
	if (render != nullptr)
		subscribers.push_back(render);
}

void RenderSystem::removeComp(Component* component)
{
	RenderComponent* render = dynamic_cast<RenderComponent*>(component);
	if (render != nullptr)
		subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), render));
}