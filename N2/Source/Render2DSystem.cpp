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
	textUID = 0;
	ui = Manager::getInstance()->getShader("ui");
	Loader::loadFont("Assets\\Fonts\\sansserif2.fnt", fonts["sansserif"]);

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


	ShaderProgram* tile = Manager::getInstance()->getShader("tile");
	tile->Use();
	tile->setUniform("view", Manager::getInstance()->getCamera()->LookAt());

	for (Render2DComponent* sub : subscribers)
	{
		if (!sub->isActive()) continue;
		ShaderProgram* shader = sub->getShader();
		Mtx44 transformation = getTransformationMatrix(sub->getParent()->getComponent<Transform2DComponent>());
		shader->setUniform("transformationMatrix", transformation);


		if (shader == tile)
		{
			shader->setUniform("textureTileSize", sub->getTextureSize());
			shader->setUniform("textureTileOffset", sub->getTextureOffset());
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sub->getTexID());
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}


	renderTexts();
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

void Render2DSystem::renderTexts()
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

void Render2DSystem::renderText(const std::string& text, float xPos, float yPos, const std::string& fontName, Vector3 color, float fontSize, TextAlignment align)
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



void Render2DSystem::generateData(const FontChar& fontChar, const float& fontSize, std::vector<UIVertex>& vertices, std::vector<unsigned int>& indices, Vector2& cursor)
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


void Render2DSystem::modifyData(const FontChar& fontChar, const int& i, const float& fontSize, std::vector<UIVertex>& vertices, std::vector<unsigned int>& indices, Vector2& cursor)
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