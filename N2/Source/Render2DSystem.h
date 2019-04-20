#ifndef RENDER2DSYSTEM_H
#define RENDER2DSYSTEM_H

#include "System.h"
#include "Render2DComponent.h"
#include "Transform2DComponent.h"
#include "Info.h"
#include "Mtx44.h"
#include "Text.h"
#include <vector>
#include <map>

class Render2DSystem : public System
{
public:
	Render2DSystem();
	~Render2DSystem();

	void Initialize();
	void Update(double& dt);

	void renderText(const std::string& text, float xPos, float yPos, const std::string& fontName, Vector3 color = Vector3(1, 1, 1), float fontSize = 1.0f, TextAlignment align = TEXT_ALIGN_LEFT);

	void registerComp(Component* component);
	void removeComp(Component* component);

private:

	void renderTexts();

	/* Text Mutation */
	void generateData(const FontChar& fontChar, const float& fontSize, std::vector<UIVertex>& vertices, std::vector<unsigned int>& indices, Vector2& cursor);
	void modifyData(const FontChar& fontChar, const int& i, const float& fontSize, std::vector<UIVertex>& vertices, std::vector<unsigned int>& indices, Vector2& cursor);


	Mtx44 getTransformationMatrix(Transform2DComponent* comp);

	Mtx44 projection;

	ShaderProgram* ui;

	std::map<std::string, Font> fonts;
	std::map<Font*, std::vector<Text*>> texts;
	std::map<unsigned int, Text> mutatedCache;
	unsigned int textUID;

	unsigned VAO, VBO, EBO;
	std::vector<Render2DComponent*> subscribers;
};

#endif