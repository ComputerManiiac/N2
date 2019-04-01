#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Info.h"

class Primitives
{
public:
	Primitives();
	~Primitives();

	static void generateCube(OBJInfo& info);
	static void generateBoxCollider(const OBJInfo& ref, std::vector<Vector3>& colliderPoints);

	static void generateQuad(OBJInfo& info);
};

#endif