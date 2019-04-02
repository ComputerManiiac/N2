#include "RenderSystem.h"
#include "Application.h"
#include "Manager.h"
#include "Entity.h"
#include "Utility.h"
#include "Primitives.h"
#include "Loader.h"


RenderSystem::RenderSystem()
{

}



RenderSystem::~RenderSystem()
{

	glDeleteBuffers(1, &batchVBO);

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

	fonts.push_back(Font());
	Loader::loadFont("Assets\\Fonts\\sansserif.fnt", fonts[0]);
	renderText("abc", 684.0f, 384.0f, fonts[0]);

	Manager* manager = Manager::getInstance();
	lit = manager->getShader("lit");
	depth = manager->getShader("depth");
	ui = manager->getShader("ui");

	/* Sets the default shader used by all light sources*/
	LightSource::setShader(lit);

	/* Bind target sampler2D to the correct sampler unit for binding textures */
	lit->Use();
	lit->setUniform("colorTexture", 0);
	lit->setUniform("depthTexture", 1);

	/* Defines projection matrix for the scene */
	projection.SetToPerspective(45.0f, (float)Application::getScreenWidth() / (float)Application::getScreenHeight(), 0.1f, 10000.0f);

	glGenBuffers(1, &batchVBO);
	BatchKey key;

	/* Generates vertex arrays and buffers for each render component and populates the assigned int into them */
 	for (RenderComponent* sub : subscribers)
	{
		unsigned int VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		setupComponent(sub->getInfo(), VAO, VBO, EBO);
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
			setupComponent(collider->getInfo(), VAO, VBO, EBO);
			collider->setBufferObjects(VAO, VBO, EBO);
		}

		/* Add To Batch */
		key.textureID = sub->getTexID();
		key.mat = mat;
		batches[key].subscribers.push_back(sub);
	}

	setupLight();
	setupShadows();
	
	

}

/* Sets up how attributes are layed out in the VAO before buffering data into VBO */
void RenderSystem::setupComponent(const OBJInfo& info, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO)
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
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(Vector3)));
}

/* Sets up values for all the lights in the Scene */
void RenderSystem::setupLight()
{
	LightSource* light = new LightSource(LIGHT_DIRECTIONAL);
	light->setDirLight(lit, Vector3(1.0f, 80.0f, 1.0f), Vector3(1, 1, 1), 1.0f);
	lightSources.push_back(light);

	LightSource* light2 = new LightSource(LIGHT_POINT);
	light2->setPointLight(lit, Vector3(10, 10.0, 0), Vector3(1, 0, 0), 1.0f, 1.0f, 0.01f, 0.001f);
	lightSources.push_back(light2);

	LightSource* light3 = new LightSource(LIGHT_SPOTLIGHT);
	light3->setSpotLight(lit, Vector3(0.1f, 10.0, 0), Vector3(1, 1, 1), Vector3(0, -1, 0), 1.0f, 45.0f, 30.0f, 1.0f, 1.0f, 0.01f, 0.001f);
	lightSources.push_back(light3);

	lit->setUniform("numLights", (int)LightSource::getCount());
}


void RenderSystem::setupShadows()
{
	shadowFBO = FrameBuffer(2048, 2048, GL_DEPTH_ATTACHMENT);

	OBJInfo info;
	Primitives::generateQuad(info);

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, info.vertices.size() * sizeof(Vertex), &info.vertices.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices.at(0), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(Vector3)));

	lightView.SetToLookAt(lightSources[0]->getPosition().x, lightSources[0]->getPosition().y, lightSources[0]->getPosition().z, 0.0f, 0.f, 0.0f, 0.0f, 1.0f, 0.0f);
	lightProjection.SetToOrtho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 80.0f);
	lightProjectionView = lightProjection * lightView;

	lit->setUniform("lightProjectionView", lightProjectionView);

	depth->Use();
	depth->setUniform("lightProjectionView", lightProjectionView);
}








/* Main loop where rendering is done */
void RenderSystem::Update(double& dt)
{
	glCullFace(GL_FRONT);
	glViewport(0, 0, 2048, 2048);
	shadowFBO.Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	renderScene(depth, lightView);

	glCullFace(GL_BACK);
	shadowFBO.Unbind();
	glViewport(0, 0, Application::getScreenWidth(), Application::getScreenHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowFBO.getTexID());
	renderScene(lit, Manager::getInstance()->getCamera()->LookAt());


	for (Text& text : texts) {
		glDeleteVertexArrays(1, &text.VAO);
	}
	texts.clear();
}

void RenderSystem::renderTexture(const FrameBuffer& buffer)
{
	ShaderProgram* quad = Manager::getInstance()->getShader("quad");
	glBindVertexArray(quadVAO);
	quad->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffer.getTexID());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void RenderSystem::renderScene(ShaderProgram* shader, const Mtx44& viewMatrix)
{
	modelStack.LoadIdentity();
	view = viewMatrix;

	/* Renders all objects in batches */
	for (auto& b : batches)
	{
		const BatchKey& key = b.first;
		Batch& batch = b.second;

		updateTransformMatrices(batch);

		/* Bind current batch's texture */
		shader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, key.textureID);

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

			if (shader == lit)
			{
				/* Drawing colliders can be costly - DEBUG ONLY! */
				ColliderComponent* collider = sub->getParent()->getComponent<ColliderComponent>();
				if (collider != nullptr && collider->shouldDrawCollider())
				{
					glBindVertexArray(collider->getVAO());
					shader->setUniform("colorTextureEnabled", 0);
					shader->setUniform("lightEnabled", false);
					glDrawElementsInstanced(GL_LINES, indexSize, GL_UNSIGNED_INT, 0, subscribers.size());
					shader->setUniform("lightEnabled", true);
					shader->setUniform("colorTextureEnabled", 1);
				}
			}
		}
	}
	
}

/* Creates a large VBO containing MVPs of objects in the current batch to draw them in a single call */
void RenderSystem::updateTransformMatrices(Batch& batch)
{
	std::vector<VertexData>& data = batch.data;

	data.clear();
	data.reserve(subscribers.size());

	glBindBuffer(GL_ARRAY_BUFFER, batchVBO);

	for (RenderComponent* sub : batch.subscribers)
	{
		modelStack.PushMatrix();

		layoutInstancedData(sub->getVAO());

		TransformComponent* transform = sub->getParent()->getComponent<TransformComponent>();
		modelStack.Translate(transform->getPos());
		modelStack.Rotate(transform->getRot());
		modelStack.Scale(transform->getScale());
		data.emplace_back(modelStack.Top(), view, projection);

		/* Drawing colliders can be costly - DEBUG ONLY! */
		ColliderComponent* collider = sub->getParent()->getComponent<ColliderComponent>();
		if (collider != nullptr && collider->shouldDrawCollider())
		{
			layoutInstancedData(collider->getVAO());
			modelStack.PushMatrix();
			modelStack.Scale(collider->getScale());
			modelStack.PopMatrix();
			data.emplace_back(modelStack.Top(), view, projection);
		}
		modelStack.PopMatrix();
	}

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(VertexData), &data.at(0), GL_STATIC_DRAW);

}

/* Defines how the instanced data should be layed out */
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




void RenderSystem::registerComp(Component* component)
{
	RenderComponent* render = static_cast<RenderComponent*>(component);
	if (render == nullptr) return;
	subscribers.push_back(render);

}

void RenderSystem::removeComp(Component* component)
{
	RenderComponent* render = static_cast<RenderComponent*>(component);
	if (render == nullptr) return;

	subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), render));
	
	BatchKey key(render->getTexID(), render->getMaterial());
	if (batches.find(key) != batches.end())
	{
		std::vector<RenderComponent*>* subs = &batches[key].subscribers;
		auto pos = std::find(subs->begin(), subs->end(), render);
		if (pos != subs->end())
			subs->erase(pos);
	}

	
}

void RenderSystem::renderText(const std::string& text, float xPos, float yPos, Font& font, float fontSize, TextAlignment align)
{
	Vector2 cursor(xPos, yPos);
	//Vector2 textDimensions;

	UIVertex vertex;

	std::vector<UIVertex> textData;
	std::vector<unsigned int> textIndices;

	float halfScreenWidth = Application::getScreenWidth() * 0.5f;
	float halfScreenHeight = Application::getScreenHeight() * 0.5f;
	float textureSize = 512.0f;


	for (int i = 0; i < (int) text.length(); i++) {
		FontChar& fontChar = font.data[(int)text[i]];


		/* 
		Screen Pixel Range, Top Left (0,0), Bottom Right (width, height)
		The extreme points are stored based on their axis such that the conversion to NDC is consistent. 
		*/
		Vector2 extremeX(cursor.x + fontChar.xOffset, cursor.x + fontChar.xOffset + fontChar.width);
		Vector2 extremeY(cursor.y + fontChar.yOffset, cursor.y + fontChar.yOffset + fontChar.height);

		/* NDC Range, Center (0,0), Top Left (-1, 1), Bottom Right (1, -1) */
		extremeX = (extremeX + 1) / halfScreenWidth - 1;
		extremeY = -((extremeY + 1) / halfScreenHeight - 1);

		/* UV Range, Bottom Left (0,0), Top Right (1,1) */
		Vector2 texCoordMin(fontChar.xPos, fontChar.yPos);
		texCoordMin /= textureSize;

		/* Y position is inverted due to the origin being at the bottom instead of top in OpenGL */
		texCoordMin.y = 1 - texCoordMin.y;

		Vector2 texCoordMax(texCoordMin.x + fontChar.width, texCoordMin.y - fontChar.height);
		texCoordMax /= textureSize;

		/* Compute the positions and uv coordinates of the quad */
		vertex.position.x = extremeX.x;
		vertex.position.y = extremeY.x;
		vertex.texCoord = texCoordMin;
		textData.push_back(vertex);

		vertex.position.y = extremeY.y;
		vertex.texCoord.y = texCoordMax.y;
		textData.push_back(vertex);
		
		vertex.position.x = extremeX.y;
		vertex.position.y = extremeY.x;
		vertex.texCoord = texCoordMin;
		vertex.texCoord.x = texCoordMax.x;
		textData.push_back(vertex);

		vertex.position.y = extremeY.y;
		vertex.texCoord = texCoordMax;
		textData.push_back(vertex);

		/* Compute the proper index count of the vertices added */
		int offset = 4 * (textData.size() / 4 - 1);
		textIndices.push_back(offset);
		textIndices.push_back(offset + 1);
		textIndices.push_back(offset + 2);
		textIndices.push_back(offset + 2);
		textIndices.push_back(offset + 1);
		textIndices.push_back(offset + 3);

		/* Advance the text cursor by set amount */
		cursor.x += fontChar.xAdvance;

		/* If the extreme right extends beyond the screen ( > 1 in NDC ), go to next line */
		if (extremeX.y > 1.0f) {
			cursor.x = 0;
			cursor.y += fontChar.height;
		}
	}

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	texts.emplace_back(textData, textIndices, VAO);
}
