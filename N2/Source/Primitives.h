#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Info.h"

class Primitives
{
public:
	Primitives();
	~Primitives();

	static void generateCubePositions(std::vector<Vector3>& positions, const float& length);
	static void generateCube(OBJInfo& info);
	static void generateBoxCollider(const OBJInfo& ref, std::vector<Vector3>& colliderPoints);

	static void generateQuad(OBJInfo& info);
	static void generateTerrain(OBJInfo& info, const std::string& heightMapPath);

private:
	static float getColourValue(const std::vector<unsigned char>& data, const int& index);
	static void calculateNormal(Vertex& self, const Vertex& a, const Vertex& b);
};

#endif