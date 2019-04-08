#include "Primitives.h"



Primitives::Primitives()
{
}


Primitives::~Primitives()
{
}


void Primitives::generateCubePositions(std::vector<Vector3>& positions, const float& length)
{
	positions.reserve(36);
	Vector3 v;

	float half = 0.5f * length;

	/* Back Face */
	v.Set(-half, half, half);
	positions.push_back(v);
	v.Set(-half, -half, half);
	positions.push_back(v);
	v.Set(half, half, half);
	positions.push_back(v);
	v.Set(half, half, half);
	positions.push_back(v);
	v.Set(-half, -half, half);
	positions.push_back(v);
	v.Set(half, -half, half);
	positions.push_back(v);


	v.Set(half, half, half);
	positions.push_back(v);
	v.Set(half, -half, half);
	positions.push_back(v);
	v.Set(half, half, -half);
	positions.push_back(v);
	v.Set(half, half, -half);
	positions.push_back(v);
	v.Set(half, -half, half);
	positions.push_back(v);
	v.Set(half, -half, -half);
	positions.push_back(v);


	/* Top Face*/
	v.Set(half, half, -half);
	positions.push_back(v);
	v.Set(-half, half, -half);
	positions.push_back(v);
	v.Set(-half, half, half);
	positions.push_back(v);
	v.Set(half, half, -half);
	positions.push_back(v);
	v.Set(-half, half, half);
	positions.push_back(v);
	v.Set(half, half, half);
	positions.push_back(v);


	/* Left Face */
	v.Set(-half, half, -half);
	positions.push_back(v);
	v.Set(-half, -half, -half);
	positions.push_back(v);
	v.Set(-half, half, half);
	positions.push_back(v);
	v.Set(-half, half, half);
	positions.push_back(v);
	v.Set(-half, -half, -half);
	positions.push_back(v);
	v.Set(-half, -half, half);
	positions.push_back(v);

	/* Bottom Face */
	v.Set(-half, -half, -half);
	positions.push_back(v);
	v.Set(half, -half, -half);
	positions.push_back(v);
	v.Set(half, -half, half);
	positions.push_back(v);
	v.Set(-half, -half, -half);
	positions.push_back(v);
	v.Set(half, -half, half);
	positions.push_back(v);
	v.Set(-half, -half, half);
	positions.push_back(v);

	/* Front Face */
	v.Set(-half, half, -half);
	positions.push_back(v);
	v.Set(half, -half, -half);
	positions.push_back(v);
	v.Set(-half, -half, -half);
	positions.push_back(v);
	v.Set(-half, half, -half);
	positions.push_back(v);
	v.Set(half, half, -half);
	positions.push_back(v);
	v.Set(half, -half, -half);
	positions.push_back(v);
}

void Primitives::generateCube(OBJInfo& info)
{
	std::vector<Vertex>& vertices = info.vertices;
	vertices.reserve(36);

	Vertex v;
	v.normal.Set(0, 0, 1);

	v.position.Set(-0.5f, 0.5f, 0.5f);
	vertices.push_back(v);

	v.position.Set(-0.5f, -0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, 0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, 0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, -0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, -0.5f, 0.5f);
	vertices.push_back(v);

	v.normal.Set(1, 0, 0);

	v.position.Set(0.5f, 0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, -0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, 0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, 0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, -0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, -0.5f, -0.5f);
	vertices.push_back(v);


	v.normal.Set(0, 1, 0);

	v.position.Set(0.5f, 0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, 0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, 0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, 0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, 0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, 0.5f, 0.5f);
	vertices.push_back(v);



	v.normal.Set(1, 0, 0);

	v.position.Set(-0.5f, 0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, -0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, 0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, 0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, -0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, -0.5f, 0.5f);
	vertices.push_back(v);

	v.normal.Set(0, -1, 0);

	v.position.Set(-0.5f, -0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, -0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, -0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, -0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, -0.5f, 0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, -0.5f, 0.5f);
	vertices.push_back(v);

	v.normal.Set(0, 0, -1);

	v.position.Set(-0.5f, 0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, -0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, -0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(-0.5f, 0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, 0.5f, -0.5f);
	vertices.push_back(v);
	v.position.Set(0.5f, -0.5f, -0.5f);
	vertices.push_back(v);

	std::vector<unsigned int>& indices = info.indices;
	indices.reserve(vertices.size());
	for (int i = 0; i < (int)vertices.size(); i++)
		indices.push_back(i);
}

void Primitives::generateBoxCollider(const OBJInfo& ref, std::vector<Vector3>& colliderPoints)
{
	Vector3 min, max;

	for (Vertex vert : ref.vertices)
	{
		const Vector3& pos = vert.position;

		// Find the two extreme corners of the object
		if (pos.x < min.x)
			min.x = pos.x;
		if (pos.y < min.y)
			min.y = pos.y;
		if (pos.z < min.z)
			min.z = pos.z;

		if (pos.x > max.x)
			max.x = pos.x;
		if (pos.y > max.y)
			max.y = pos.y;
		if (pos.z > max.z)
			max.z = pos.z;
	}

	// Create all 8 extreme points of the box collider
	Vector3 v = min;
	colliderPoints.push_back(v);
	v = max;
	colliderPoints.push_back(v);

	v = min;
	v.x = max.x;
	colliderPoints.push_back(v);
	v = min;
	v.y = max.y;
	colliderPoints.push_back(v);
	v = min;
	v.z = max.z;
	colliderPoints.push_back(v);

	v = max;
	v.x = min.x;
	colliderPoints.push_back(v);
	v = max;
	v.y = min.y;
	colliderPoints.push_back(v);
	v = max;
	v.z = min.z;
	colliderPoints.push_back(v);
}



void Primitives::generateQuad(OBJInfo& info)
{
	std::vector<Vertex>& vertices = info.vertices;

	float length = 2.0f;
	float halfLength = 0.5f * length;

	Vertex v;
	v.normal.Set(0, 0, 1);

	v.position.Set(-halfLength, halfLength, 0);
	v.texCoord.Set(0, 1);
	vertices.push_back(v);


	v.position.Set(-halfLength, -halfLength, 0);
	v.texCoord.Set(0, 0);
	vertices.push_back(v);


	v.position.Set(halfLength, halfLength, 0);
	v.texCoord.Set(1, 1);
	vertices.push_back(v);


	v.position.Set(halfLength, -halfLength, 0);
	v.texCoord.Set(1, 0);
	vertices.push_back(v);

	info.indices = { 0,1,2,2,1,3 };

}