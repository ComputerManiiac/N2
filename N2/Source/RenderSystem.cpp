#include "RenderSystem.h"
#include "Application.h"
#include "Manager.h"
#include "Entity.h"
#include "Utility.h"
#include "Primitives.h"
#include "Loader.h"
#include "RendererLit.h"
#include "RendererShadow.h"
#include "RendererGrass.h"
#include "RendererTerrain.h"
#include "RendererSkybox.h"




RenderSystem::RenderSystem()
{

}



RenderSystem::~RenderSystem()
{

	


	/* Deinitialize batches with the appropriate render */
	for (auto& b : batches)
	{
		const BatchKey& key = b.first;
		Batch& batch = b.second;
		renderers[key.shader]->Deinitialize(batch);
	}

	/* Free cached textures */
	std::map<std::string, unsigned int>& cachedTextures = Loader::getCachedTextures();
	for (auto& t : cachedTextures)
		glDeleteTextures(1, &t.second);

	/* Free renderers */
	for (auto& r : renderers)
	{
		const Renderer* renderer = r.second;
		delete r.second;

	}

	/* Free buffer objects for Text */
	for (auto& m : mutatedCache)
	{
		const Text& t = m.second;
		glDeleteVertexArrays(1, &t.VAO);
		glDeleteBuffers(1, &t.VBO);
		glDeleteBuffers(1, &t.EBO);
	}

	/* Free Light Sources */
	for (LightSource* light : lightSources)
		delete light;
}

void RenderSystem::Initialize() {

	textUID = 0;
	 
	Loader::loadFont("Assets\\Fonts\\sansserif2.fnt", fonts["sansserif"]);

	/* Defines projection matrix for the scene */
	projection.SetToPerspective(45.0f, (float)Application::getScreenWidth() / (float)Application::getScreenHeight(), 0.1f, 10000.0f);

	Manager* manager = Manager::getInstance();
	lit = manager->getShader("lit");
	depth = manager->getShader("depth");
	ui = manager->getShader("ui");
	ShaderProgram* grass = manager->getShader("grass");
	ShaderProgram* particleShader = manager->getShader("particle");
	ShaderProgram* terrain = manager->getShader("terrain");
	ShaderProgram* skybox = manager->getShader("skybox");
	ShaderProgram* water = manager->getShader("water");

	/* Set renderers */
	renderers[grass] = new RendererGrass(grass);
	renderers[lit] = new RendererLit(lit);
	renderers[depth] = new RendererShadow(depth);
	renderers[terrain] = new RendererTerrain(terrain);
	renderers[skybox] = new RendererSkybox(skybox);
	waterRenderer = new RendererWater(water);
	renderers[water] = waterRenderer;

	particle = new RendererParticle(particleShader);
	renderers[particleShader] = particle;
	particle->Initialize();


	BatchKey key;

	/* Generates vertex arrays and buffers for each render component and populates the assigned int into them */
 	for (RenderComponent* sub : subscribers)
	{
		ShaderProgram* componentShader = sub->getShader();

		/* Add To Batch */
		key.setAll(componentShader, sub->getTexID(), sub->getMaterial());

		if (batches.find(key) == batches.end() || batches[key].info.vertices.size() == 0)
			batches[key].info = sub->getInfo();

		batches[key].subscribers.push_back(sub);
	}

	for (auto& b : batches)
	{
		const BatchKey& key = b.first;
		Batch& batch = b.second;
		renderers[key.shader]->Initialize(key, batch);
	}

	glBindVertexArray(0);
	setupLight();
	setupShadows();
	

}


void RenderSystem::setupLight()
{
	LightSource::setShaders({ lit, Manager::getInstance()->getShader("grass"),
		Manager::getInstance()->getShader("terrain"), Manager::getInstance()->getShader("water")});

	LightSource* light = new LightSource(LIGHT_DIRECTIONAL);
	light->setDirLight(lit, Vector3(2.0f, 100.0f, 2.0f), Vector3(1, 1, 1), 1.0f);
	lightSources.push_back(light);

	LightSource* light2 = new LightSource(LIGHT_POINT);
	light2->setPointLight(lit, Vector3(10, 10.0, 0), Vector3(1, 0, 0), 1.0f, 1.0f, 0.01f, 0.001f);
	lightSources.push_back(light2);

	LightSource* light3 = new LightSource(LIGHT_SPOTLIGHT);
	light3->setSpotLight(lit, Vector3(0.1f, 10.0, 0), Vector3(1, 1, 1), Vector3(0, -1, 0), 1.0f, 45.0f, 30.0f, 1.0f, 1.0f, 0.01f, 0.001f);
	lightSources.push_back(light3);

	
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
	lightProjection.SetToOrtho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 500.0f);
	lightProjectionView = lightProjection * lightView;

	lit->Use();
	lit->setUniform("lightProjectionView", lightProjectionView);

	ShaderProgram* grass = Manager::getInstance()->getShader("grass");
	grass->Use();
	grass->setUniform("lightProjectionView", lightProjectionView);

	depth->Use();
	depth->setUniform("lightProjectionView", lightProjectionView);

	ShaderProgram* terrain = Manager::getInstance()->getShader("terrain");
	terrain->Use();
	terrain->setUniform("lightProjectionView", lightProjectionView);

	glBindVertexArray(0);
}


void RenderSystem::Update(double& dt)
{
	float renderTime = 0.0f;
	{
		/* Used for timing render loop */
		PerformanceClock clock(&renderTime);

		/* Update batched data once which is then reused in each renderScene pass */
		updateBatchedData();

		/* Render scene using simple depth shader to get depth map */
		glCullFace(GL_FRONT);
		glViewport(0, 0, 2048, 2048);
		shadowFBO.Bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		renderScene(lightView, depth, false);


		glCullFace(GL_BACK);
		shadowFBO.Unbind();

		///* Render scene to reflection texture */
		glViewport(0, 0, Application::getScreenWidth(), Application::getScreenHeight());
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//waterRenderer->bindReflectionFBO();
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, shadowFBO.getTexID());
		//renderScene(Manager::getInstance()->getCamera()->LookAt(), nullptr, false);

		/* Render scene normally */
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowFBO.getTexID());
		renderScene(Manager::getInstance()->getCamera()->LookAt(), nullptr, true);
	}

	std::string renderTimeString = std::to_string(renderTime);
	renderTimeString = renderTimeString.substr(0, renderTimeString.find_first_of('.')+2 );

	/* Render texts */
	glDisable(GL_DEPTH_TEST);
	renderText("FPS: " + std::to_string(Application::framesPerSecond), 10, 0, "sansserif", Vector3(1, 1, 0), 0.30f);
	renderText("Draw Calls: " + std::to_string(batches.size() * 2), 10, 30, "sansserif", Vector3(1, 0, 1), 0.30f);
	renderText("Object Count: " + std::to_string(subscribers.size()), 10, 60, "sansserif", Vector3(0, 1, 0), 0.30f);
	renderText("Render Loop: " + renderTimeString + "ms", 10, 90, "sansserif", Vector3(1, 0, 0), 0.28f);
	renderTexts();
	glEnable(GL_DEPTH_TEST);
}

void RenderSystem::updateBatchedData()
{
	for (auto& b : batches)
	{
		const BatchKey& key = b.first;
		Batch& batch = b.second;


		//batch.data.clear();
		//batch.data.reserve(batch.subscribers.size());

		for(int i = 0; i < (int) batch.subscribers.size(); i++)
		//for (RenderComponent* sub : batch.subscribers)
		{
			RenderComponent* sub = batch.subscribers[i];
			modelStack.PushMatrix();
			TransformComponent* transform = sub->getParent()->getComponent<TransformComponent>();
			modelStack.Translate(transform->getPos());
			modelStack.Rotate(transform->getRot());
			modelStack.Scale(transform->getScale());

			/* Update MVP */
			if (i < batch.modelMatrices.size())
				batch.modelMatrices[i] = modelStack.Top();
			else
				batch.modelMatrices.push_back(modelStack.Top());

			if (i < batch.data.size())
				batch.data[i].model = modelStack.Top();
			else
				batch.data.emplace_back(modelStack.Top());


			modelStack.PopMatrix();
		}

		/* Update model info for each batch for the shadow renderer */
		static_cast<RendererShadow*>(renderers[depth])->setModelMatricesForBatch(&batch, batch.modelMatrices);
	}
}

void RenderSystem::renderScene(const Mtx44& viewMatrix, ShaderProgram* shader, bool renderWater)
{
	lit->Use();
	modelStack.LoadIdentity();

	/* Renders all objects in batches */
	for (auto& b : batches)
	{
		const BatchKey& key = b.first;
		Batch& batch = b.second;

		

		if (shader == nullptr) {
			key.shader->Use();

			if (!renderWater && key.shader == Manager::getInstance()->getShader("water"))
				continue;

			renderers[key.shader]->Render(batch, key.textureID, modelStack, viewMatrix);

		} else {

			shader->Use();
			renderers[shader]->Render(batch, key.textureID, modelStack, viewMatrix);

			
			
		}
	}
	glBindVertexArray(0);
}



void RenderSystem::renderTexts()
{
	for (auto& f : texts)
	{
		const Font& font = *f.first;
		std::vector<Text*>& text = f.second;
		ui->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, font.textureID);
	
		for (Text* t : text)
		{
			ui->setUniform("textColor", t->color);
			ui->setUniform("textSize", t->textSize);
			glBindVertexArray(t->VAO);
			glDrawElements(GL_TRIANGLES, t->indices.size(), GL_UNSIGNED_INT, 0);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(0);
	texts.clear();
	textUID = 0;
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

void RenderSystem::renderText(const std::string& text, float xPos, float yPos, const std::string& fontName, Vector3 color, float fontSize, TextAlignment align)
{
	++textUID;

	Font& font = fonts[fontName];

	UIVertex vertex;
	Vector2 cursor(xPos, yPos);

	/* New mutated text not in cache */
	if (mutatedCache.find(textUID) == mutatedCache.end())
	{

		/* Generate all data needed for rendering */
		std::vector<UIVertex> vertices;
		std::vector<unsigned int> indices;
		for (int i = 0; i < (int)text.length(); i++)
		{
			FontChar& fontChar = font.data[(int)text[i]];
			generateData(fontChar, fontSize, vertices, indices, cursor);
		}

		unsigned int VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(UIVertex), &vertices.at(0), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.at(0), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(UIVertex), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(UIVertex), (void*) sizeof(Vector2));

		Text& t = mutatedCache[textUID];
		t.setAll(text, fontSize, vertices, indices, color, VAO, VBO, EBO);

		texts[&font].push_back(&mutatedCache[textUID]);
	}
	/* Existing text in cache */
	else
	{

		Text& t = mutatedCache[textUID];

		/* Existing data of the old text */
		std::vector<UIVertex>& vertices = t.vertices;
		std::vector<unsigned int>& indices = t.indices;

		if (t.raw != text)
		{
			for (int i = 0; i < text.length(); i++)
			{
				/* Get information about current character in new string */
				const FontChar& fontChar = font.data[(int)text[i]];

				/* Check if character exists in the old string */
				if (i < (int)t.raw.length())
				{
					/* Skip if character is same */
					if (text[i] == t.raw[i])
					{
						cursor.x += fontChar.xAdvance * fontSize;
						if (cursor.x > Application::getScreenWidth())
						{
							cursor.x = xPos;
							cursor.y += fontChar.height;
						}
						continue;
					}

					/* If a different character is present, modify existing data to match the new character*/
					int vertexIndex = i * 4;
					modifyData(fontChar, vertexIndex, fontSize, vertices, indices, cursor);
				}
				else
				{
					/* Generate data for new character */
					generateData(fontChar, fontSize, vertices, indices, cursor);
				}

				/* If the cursor extends beyond the screen ( > 1 in NDC ), go to next line */
				if (cursor.x > Application::getScreenWidth())
				{
					cursor.x = xPos;
					cursor.y += fontChar.height;
				}
			}

			/* If the new mutated text is smaller than the previous cached text, then delete unnecessary data */
			int diff = t.raw.length() - text.length();
			if (diff > 0)
			{
				int v = diff * 4;
				for (int i = 0; i < v; ++i)
					vertices.pop_back();

				int e = diff * 6;
				for (int i = 0; i < e; ++i)
					indices.pop_back();
			}

			/* Set cached text to be the new text */
			t.raw = text;
		}

		glBindVertexArray(t.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, t.VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(UIVertex), &vertices.at(0), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.at(0), GL_STATIC_DRAW);

		texts[&font].push_back(&mutatedCache[textUID]);
	}
}

const Mtx44 RenderSystem::getProjectionMatrix() const
{
	return projection;
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
	
	BatchKey key(render->getShader(), render->getTexID(), render->getMaterial());
	if (batches.find(key) != batches.end())
	{
		std::vector<RenderComponent*>* subs = &batches[key].subscribers;
		auto pos = std::find(subs->begin(), subs->end(), render);
		if (pos != subs->end())
			subs->erase(pos);
	}

	
}



void RenderSystem::generateData(const FontChar& fontChar, const float& fontSize, std::vector<UIVertex>& vertices, std::vector<unsigned int>& indices, Vector2& cursor)
{
	Vector2 extremeX(cursor.x + fontChar.xOffset * fontSize, cursor.x + fontChar.xOffset * fontSize + fontChar.width * fontSize);
	Vector2 extremeY(cursor.y + fontChar.yOffset * fontSize, cursor.y + fontChar.yOffset * fontSize + fontChar.height * fontSize);

	/* NDC Range, Center (0,0), Top Left (-1, 1), Bottom Right (1, -1) */
	float halfScreenWidth = Application::getScreenWidth() * 0.5f;
	float halfScreenHeight = Application::getScreenHeight() * 0.5f;

	extremeX = (extremeX + 1) / halfScreenWidth - 1;
	extremeY = -((extremeY + 1) / halfScreenHeight - 1);

	UIVertex vertex;

	/* Compute the positions and uv coordinates of the character's quad */
	vertex.position.x = extremeX.x;
	vertex.position.y = extremeY.x;
	vertex.texCoord = fontChar.texCoordMin;
	vertices.push_back(vertex);


	vertex.position.y = extremeY.y;
	vertex.texCoord.y = fontChar.texCoordMax.y;
	vertices.push_back(vertex);

	vertex.position.x = extremeX.y;
	vertex.position.y = extremeY.x;
	vertex.texCoord = fontChar.texCoordMin;
	vertex.texCoord.x = fontChar.texCoordMax.x;
	vertices.push_back(vertex);

	vertex.position.y = extremeY.y;
	vertex.texCoord = fontChar.texCoordMax;
	vertices.push_back(vertex);

	int offset = 4 * (vertices.size() / 4 - 1);

	indices.push_back(offset);
	indices.push_back(offset + 1);
	indices.push_back(offset + 2);
	indices.push_back(offset + 2);
	indices.push_back(offset + 1);
	indices.push_back(offset + 3);

	/* Advance the text cursor by set amount */
	cursor.x += fontChar.xAdvance * fontSize;
}


void RenderSystem::modifyData(const FontChar& fontChar, const int& i, const float& fontSize, std::vector<UIVertex>& vertices, std::vector<unsigned int>& indices, Vector2& cursor)
{
	Vector2 extremeX(cursor.x + fontChar.xOffset * fontSize, cursor.x + fontChar.xOffset * fontSize + fontChar.width * fontSize);
	Vector2 extremeY(cursor.y + fontChar.yOffset * fontSize, cursor.y + fontChar.yOffset * fontSize + fontChar.height * fontSize);
	/* NDC Range, Center (0,0), Top Left (-1, 1), Bottom Right (1, -1) */

	float halfScreenWidth = Application::getScreenWidth() * 0.5f;
	float halfScreenHeight = Application::getScreenHeight() * 0.5f;

	extremeX = (extremeX + 1) / halfScreenWidth - 1;
	extremeY = -((extremeY + 1) / halfScreenHeight - 1);


	UIVertex vertex;

	/* Compute the positions and uv coordinates of the character's quad */
	vertex.position.x = extremeX.x;
	vertex.position.y = extremeY.x;
	vertex.texCoord = fontChar.texCoordMin;
	vertices[i] = vertex;

	vertex.position.y = extremeY.y;
	vertex.texCoord.y = fontChar.texCoordMax.y;
	vertices[i + 1] = vertex;

	vertex.position.x = extremeX.y;
	vertex.position.y = extremeY.x;
	vertex.texCoord = fontChar.texCoordMin;
	vertex.texCoord.x = fontChar.texCoordMax.x;
	vertices[i + 2] = vertex;

	vertex.position.y = extremeY.y;
	vertex.texCoord = fontChar.texCoordMax;
	vertices[i + 3] = vertex;

	/* Advance the text cursor by set amount */
	cursor.x += fontChar.xAdvance * fontSize;
}


//ColliderComponent* collider = sub->getParent()->getComponent<ColliderComponent>();
//if (collider != nullptr)
//{
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	setupComponent(collider->getInfo(), VAO, VBO, EBO);
//	collider->setBufferObjects(VAO, VBO, EBO);
//}


			//if (shader == lit)
			//{
			//	/* Drawing colliders can be costly - DEBUG ONLY! */
			//	ColliderComponent* collider = sub->getParent()->getComponent<ColliderComponent>();
			//	if (collider != nullptr && collider->shouldDrawCollider())
			//	{
			//		glBindVertexArray(collider->getVAO());
			//		shader->setUniform("colorTextureEnabled", 0);
			//		shader->setUniform("lightEnabled", false);
			//		glDrawElementsInstanced(GL_LINES, indexSize, GL_UNSIGNED_INT, 0, subscribers.size());
			//		shader->setUniform("lightEnabled", true);
			//		shader->setUniform("colorTextureEnabled", 1);
			//	}
			//}


/* Sets up how attributes are layed out in the VAO before buffering data into VBO */
//void RenderSystem::setupComponent(const OBJInfo& info, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO)
//{
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, info.vertices.size() * sizeof(Vertex), &info.vertices.at(0), GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices.at(0), GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glEnableVertexAttribArray(2);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Vector3));
//	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(Vector3)));
//
//}
		/* Drawing colliders can be costly - DEBUG ONLY! */
//ColliderComponent* collider = sub->getParent()->getComponent<ColliderComponent>();
//if (collider != nullptr && collider->shouldDrawCollider())
//{
//	layoutComponentInstancedData(collider->getVAO());
//	modelStack.PushMatrix();
//	modelStack.Scale(collider->getScale());
//	modelStack.PopMatrix();
//	data.emplace_back(modelStack.Top(), view, projection);
//}