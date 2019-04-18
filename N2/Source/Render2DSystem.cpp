#include "Render2DSystem.h"
#include "Application.h"
#include "Primitives.h"
#include "Manager.h"
#include "Loader.h"
#include <algorithm>


Render2DSystem::Render2DSystem()
{
}


Render2DSystem::~Render2DSystem()
{
}

void Render2DSystem::Initialize()
{

	projection.SetToOrtho(0.0f, Application::getScreenWidth(), 0.0f, Application::getScreenHeight(), 0.0f, 100.0f);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	OBJInfo2D info;
	Primitives::generateQuad2D(info);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, info.vertices.size() * sizeof(Vector3), &info.vertices.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices.at(0), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vector3), (void*)0);

	glBindVertexArray(0);

	ShaderProgram* shader = Manager::getInstance()->getShader("tile");
	shader->Use();
	shader->setUniform("tileTexture", 0);
	shader->setUniform("projection", projection);
}

void Render2DSystem::Update(double& dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	ShaderProgram* shader = Manager::getInstance()->getShader("tile");
	shader->Use();
	shader->setUniform("view", Manager::getInstance()->getCamera()->LookAt());

	for (Render2DComponent* sub : subscribers)
	{
		Mtx44 transformation = getTransformationMatrix(sub->getParent()->getComponent<Transform2DComponent>());
		shader->setUniform("transformationMatrix", transformation);
		shader->setUniform("textureTileOffset", sub->getTextureOffset());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sub->getTexID());
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
}

void Render2DSystem::registerComp(Component* component)
{
	Render2DComponent* render = static_cast<Render2DComponent*>(component);
	if (render == nullptr) return;
	subscribers.push_back(render);
}

void Render2DSystem::removeComp(Component* component)
{
	Render2DComponent* render = static_cast<Render2DComponent*>(component);
	if (render == nullptr) return;

	subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), render));

	//BatchKey key(render->getShader(), render->getTexID(), render->getMaterial());
	//if (batches.find(key) != batches.end())
	//{
	//	std::vector<RenderComponent*>* subs = &batches[key].subscribers;
	//	auto pos = std::find(subs->begin(), subs->end(), render);
	//	if (pos != subs->end())
	//		subs->erase(pos);
	//}
}

Mtx44 Render2DSystem::getTransformationMatrix(Transform2DComponent* comp)
{

	Mtx44 scale;
	scale.SetToScale(comp->getScale().x, comp->getScale().y, 1);

	Mtx44 rotation;
	rotation.SetToRotation(comp->getRot(), 0, 0, 1);

	Mtx44 transform;
	transform.SetToTranslation(comp->getCenterPos().x, Application::getScreenHeight() - comp->getCenterPos().y, comp->getPos().z);

	return  transform * rotation * scale;
}
