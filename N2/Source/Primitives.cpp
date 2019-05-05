#include "Primitives.h"
#include "Loader.h"
#include "Manager.h"


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

	float length = 1.0f;
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

void Primitives::generateTerrain(OBJInfo& info, const std::string& heightMapPath, const float& minHeight, const float& maxHeight, const float& cellLength)
{
	std::vector<unsigned char> data;
	Loader::loadBMP(heightMapPath, data);



	float gridLength = static_cast<float>(sqrt(data.size() / 3));
	float cellLengthUV = 1.0f / (gridLength-1);

	float halfGridLength = gridLength * 0.5f * cellLength;


	Vector2 offset;
	offset.y = -halfGridLength;
	Vector2 textureCoords;

	Vertex v;
	v.normal.Set(0, 1, 0);
	
	info.vertices.reserve(gridLength * gridLength);

	/* Vertices */
	for (int z = 0; z < gridLength; z++) {
		offset.x = -halfGridLength;
		textureCoords.x = 0.0f;
		for (int x = 0; x < gridLength; x++) {

			/* Y Offset = Average color value of pixel in height map * constant */
			int vertIndex = z * gridLength + x;
			int pixelIndex = vertIndex * 3;
			float averageColorOfPixel = getColourValue(data, pixelIndex);
			float yOffset = minHeight + (averageColorOfPixel / 255.0f) * (maxHeight - minHeight);
			

			v.position.Set(offset.x, yOffset, offset.y);
			v.texCoord.Set(textureCoords.x, textureCoords.y);

			info.vertices.push_back(v);

			if (x != 0 && z != 0)
				calculateNormal(info.vertices[vertIndex], info.vertices[vertIndex - 1], info.vertices[vertIndex - gridLength]);

			offset.x += cellLength;
			textureCoords.x += cellLengthUV;
		}
		offset.y += cellLength;
		textureCoords.y += cellLengthUV;
	}

	/* Border Vertices */
	for (int z = 0; z < gridLength-1; z++) {
		for (int x = 0; x < gridLength; x++) {
			if (x == 0 || z == 0) {
				int vertIndex = z * gridLength + x;
				calculateNormal(info.vertices[vertIndex], info.vertices[vertIndex + 1], info.vertices[vertIndex + gridLength]);
			}
		}
	}

	/* Bottom Left Vertex */
	int index = gridLength * (gridLength - 1);
	calculateNormal(info.vertices[index], info.vertices[index - gridLength], info.vertices[index + 1]);

	/* Indices */
	int lastVert = gridLength - 1;
	int totalQuads = lastVert * lastVert;
	info.indices.reserve(totalQuads * 6);

	for (int x = 0; x < lastVert; ++x) {
		for (int z = 0; z < gridLength; ++z) {

			int vertCount = x * gridLength + z;
			if (z != lastVert) {
				info.indices.push_back(vertCount);
				info.indices.push_back(vertCount + gridLength);
				info.indices.push_back(vertCount + 1);
				info.indices.push_back(vertCount + 1);
				info.indices.push_back(vertCount + gridLength);
				info.indices.push_back(vertCount + gridLength + 1);
			}
		}

	}
}

float Primitives::getColourValue(const std::vector<unsigned char>& data, const int & index)
{
	return (data[index] + data[index+1] + data[index+2]) / 3.0f;
}

void Primitives::calculateNormal(Vertex &self, const Vertex& a, const Vertex &b)
{
	Vector3 one = a.position - self.position;
	Vector3 two = b.position - self.position;
	self.normal = one.Cross(two).Normalize();
}

void Primitives::generateSkyplane(OBJInfo& info, int slices, const float& planetRadius, const float& atmosphereRadius, const float& hTile, const float& vTile)
{
	std::vector<Vertex>& vertex_buffer_data = info.vertices;
	std::vector<unsigned int>& index_buffer_data = info.indices;
	
	//Slice only from 1 - 256
	slices = Math::Clamp(slices, 1, 256);
	
	//PlanetRaidus is xRadius, AtmosphereRadius is yRadius
	float planeSize = 2.f * (float)sqrtf(planetRadius * planetRadius + atmosphereRadius * atmosphereRadius);
	//Get values for traveling
	float delta = planeSize / (float)slices;
	float texDelta = 2.f / (float)slices;
	
	//Get Vertices
	//Travel along the Z axis
	for (int z = 0; z <= slices; ++z)
	{
		//Travel along the X axis
		for (int x = 0; x <= slices; ++x)
		{
			//The X and Z position based on the current x and z value
			float xDist = (-0.5f *planeSize) + ((float)x *delta);
			float zDist = (-0.5f *planeSize) + ((float)z *delta);
	
			//Getting the height
			float xHeight = (xDist * xDist) / atmosphereRadius;
			float zHeight = (zDist * zDist) / atmosphereRadius;
			float height = xHeight + zHeight;
	
			Vertex v;
			//set pos
			v.position.Set(xDist, 0.f - height, zDist);
			//set TextureCoord
			float texU = hTile * ((float)x * texDelta * 0.5f);
			float texV = vTile * (1.f - (float)z * texDelta * 0.5f);
			v.texCoord.Set(texU, texV);
	
			vertex_buffer_data.push_back(v);
		}
	}
	
	//Calculating the indices
	int index = 0;
	for (int i = 0; i < slices; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			int startVert = (i * (slices + 1) + j);
			//First Triangle of the quad
			index_buffer_data.push_back(startVert);
			index_buffer_data.push_back(startVert + 1);
			index_buffer_data.push_back(startVert + slices + 1);
			//Seoond Triangle of the quad
			index_buffer_data.push_back(startVert + 1);
			index_buffer_data.push_back(startVert + slices + 2);
			index_buffer_data.push_back(startVert + slices + 1);
		}
	}


}


//
//Mesh * MeshBuilder::GenerateSkyPlane(const std::string & meshName, Color color, int slices, float PlanetRadius, float AtmosphereRadius, float hTile, float vTile)
//{
//	std::vector<Vertex> vertex_buffer_data;
//	std::vector<GLuint> index_buffer_data;
//
//	//Slice only from 1 - 256
//	slices = Math::Clamp(slices, 1, 256);
//
//	//PlanetRaidus is xRadius, AtmosphereRadius is yRadius
//	float planeSize = 2.f * (float)sqrtf(PlanetRadius * PlanetRadius + AtmosphereRadius * AtmosphereRadius);
//	//Get values for traveling
//	float delta = planeSize / (float)slices;
//	float texDelta = 2.f / (float)slices;
//
//	//Get Vertices
//	//Travel along the Z axis
//	for (int z = 0; z <= slices; ++z)
//	{
//		//Travel along the X axis
//		for (int x = 0; x <= slices; ++x)
//		{
//			//The X and Z position based on the current x and z value
//			float xDist = (-0.5f *planeSize) + ((float)x *delta);
//			float zDist = (-0.5f *planeSize) + ((float)z *delta);
//
//			//Getting the height
//			float xHeight = (xDist * xDist) / AtmosphereRadius;
//			float zHeight = (zDist * zDist) / AtmosphereRadius;
//			float height = xHeight + zHeight;
//
//			Vertex v;
//			//set pos
//			v.pos.Set(xDist, 0.f - height, zDist);
//			//set color
//			v.color = color;
//			//set TextureCoord
//			float texU = hTile * ((float)x * texDelta * 0.5f);
//			float texV = vTile * (1.f - (float)z * texDelta * 0.5f);
//			v.texCoord.Set(texU, texV);
//
//			vertex_buffer_data.push_back(v);
//		}
//	}
//
//	//Calculating the indices
//	int index = 0;
//	for (int i = 0; i < slices; ++i)
//	{
//		for (int j = 0; j < slices; ++j)
//		{
//			int startVert = (i * (slices + 1) + j);
//			//First Triangle of the quad
//			index_buffer_data.push_back(startVert);
//			index_buffer_data.push_back(startVert + 1);
//			index_buffer_data.push_back(startVert + slices + 1);
//			//Seoond Triangle of the quad
//			index_buffer_data.push_back(startVert + 1);
//			index_buffer_data.push_back(startVert + slices + 2);
//			index_buffer_data.push_back(startVert + slices + 1);
//		}
//	}
//
//	Mesh *mesh = new Mesh(meshName);
//
//	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
//	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
//
//	mesh->indexSize = index_buffer_data.size();
//	mesh->mode = Mesh::DRAW_TRIANGLES;
//
//	return mesh;
//}