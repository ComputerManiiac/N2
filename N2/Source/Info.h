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

	friend std::ostream& operator<<(std::ostream& os, const Vector2& rhs)
	{
		os << "(" << rhs.x << "," << rhs.y << ")";
		return os;
	}
	float x;
	float y;
};


struct Vertex {


	Vertex(Vector3 position, Vector2 texCoord, Vector3 normal) : position(position), texCoord(texCoord), normal(normal) {}
	Vertex() {}

	bool operator < (const Vertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
	}

	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;

};




struct OBJInfo {


	OBJInfo(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {}
	OBJInfo() {}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};


struct OBJInfo2D {

	OBJInfo2D(std::vector<Vector3> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {}
	OBJInfo2D() {}

	std::vector<Vector3> vertices;
	std::vector<unsigned int> indices;
};
#endif