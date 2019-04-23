#ifndef OBJINFO_H
#define OBJINFO_H

#include "Vector3.h"
#include <vector>


struct Vector2 {

	Vector2() : x(0), y(0) {}
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





#endif