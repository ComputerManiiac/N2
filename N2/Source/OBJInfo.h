#ifndef OBJINFO_H
#define OBJINFO_H

#include "Vector3.h"
#include <vector>

struct Vertex {



	Vector3 position;
	Vector3 normal;
	Vector3 texCoord;

	Vertex(Vector3 position, Vector3 texCoord, Vector3 normal) : position(position), texCoord(texCoord), normal(normal) {}
	Vertex() {}

	inline bool operator < (const Vertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
	}

};



struct OBJInfo {

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	OBJInfo(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {}

	OBJInfo(){}


};

#endif