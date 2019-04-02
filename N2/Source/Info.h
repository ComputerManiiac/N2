#ifndef OBJINFO_H
#define OBJINFO_H

#include "Vector3.h"
#include <vector>


struct Vector2 {

	Vector2() {}
	Vector2(const Vector2& other) : x(other.x), y(other.y) {}
	Vector2(float x, float y) : x(x), y(y) {}

	void Set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2& operator=(const Vector2& other) {
		x = other.x;
		y = other.y;
		return *this;
	}

	Vector2& operator+(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& operator+(const float& other) {
		x += other;
		y += other;
		return *this;
	}

	Vector2& operator-(const Vector2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2& operator-(const float& other) {
		x -= other;
		y -= other;
		return *this;
	}

	Vector2& operator*(const float &other) {
		x *= other;
		y *= other;
		return *this;
	}

	Vector2& operator/(const float& other) {
		x /= other;
		y /= other;
		return *this;
	}

	Vector2& operator/=(const float& other) {
		x /= other;
		y /= other;
		return *this;
	}

	Vector2& operator-() {
		x = -x;
		y = -y;
		return *this;
	}

	float x;
	float y;
};


struct Vertex {



	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;

	Vertex(Vector3 position, Vector2 texCoord, Vector3 normal) : position(position), texCoord(texCoord), normal(normal) {}
	Vertex() {}

	bool operator < (const Vertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
	}

};


struct OBJInfo {

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	OBJInfo(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {}

	OBJInfo() {}

};



struct FontChar {
	
	FontChar() : id(0), xPos(-1), yPos(-1), width(-1), height(-1), xOffset(-1), yOffset(-1) {}

	FontChar(char id, float xPos, float yPos, float width, float height, float xOffset, float yOffset)
		: id(id), xPos(xPos), yPos(yPos), width(width), height(height), xOffset(xOffset), yOffset(yOffset) {}

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
};

struct Font {

	Font() {}
	unsigned int textureID;
	FontChar data[256];
};

struct UIVertex {

	UIVertex() {}
	UIVertex(Vector2 position, Vector2 texCoord) : position(position), texCoord(texCoord){}

	Vector2 position;
	Vector2 texCoord;
};

struct Text {

	Text() {}
	Text(std::vector<UIVertex> vertices, std::vector<unsigned int> indices, unsigned int VAO) : vertices(vertices), indices(indices), VAO(VAO) {}

	std::vector<UIVertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;
};



#endif