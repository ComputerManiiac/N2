#ifndef TEXT_H
#define TEXT_H


#include "Vector3.h"
#include "Info.h"
#include <vector>

struct FontChar {

	FontChar() : id(0), xPos(-1), yPos(-1), width(-1), height(-1), xOffset(-1), yOffset(-1), xAdvance(-1) {}

	FontChar(char id, float xPos, float yPos, float width, float height, float xOffset, float yOffset, float xAdvance, Vector2 texCoordMin, Vector2 texCoordMax)
		: id(id), xPos(xPos), yPos(yPos), width(width), height(height), xOffset(xOffset), yOffset(yOffset), xAdvance(xAdvance), texCoordMin(texCoordMin), texCoordMax(texCoordMax)
	{
	}

	// ASCII equivalent number
	char id;

	// X and Y offsets for the starting position of the character in the font texture with origin at top left
	float xPos;
	float yPos;

	// Width and Height of the character in the font texture
	float width;
	float height;

	// X and Y offsets relative to the text cursor where the character is supposed to be drawn
	float xOffset;
	float yOffset;

	/* How much the text cursor should advance in the x-axis after drawing this char*/
	float xAdvance;

	Vector2 texCoordMin;
	Vector2 texCoordMax;
};

struct Font {

	Font() {}
	unsigned int textureID;
	FontChar data[128];
};

struct UIVertex {

	UIVertex() {}
	UIVertex(Vector2 position, Vector2 texCoord) : position(position), texCoord(texCoord) {}

	Vector2 position;
	Vector2 texCoord;

};

struct Text {

	Text() {}

	Text(const std::string& raw, const float& textSize, const std::vector<UIVertex>& vertices, const std::vector<unsigned int>& indices, const Vector3& color, const unsigned int& VAO, const unsigned int& VBO,
		const unsigned int& EBO)
		: raw(raw), textSize(textSize), vertices(vertices), indices(indices), VAO(VAO), VBO(VBO), EBO(EBO), color(color)
	{
	}


	void setAll(const std::string& raw, const float& textSize, const std::vector<UIVertex>& vertices, const std::vector<unsigned int>& indices, const Vector3& color, const unsigned int& VAO, const unsigned int& VBO,
		const unsigned int& EBO)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->VAO = VAO;
		this->VBO = VBO;
		this->EBO = EBO;
		this->color = color;
		this->raw = raw;
		this->textSize = textSize;
	}

	std::vector<UIVertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	Vector3 color;
	float textSize;
	std::string raw;
};



#endif