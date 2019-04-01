#include "Loader.h"


std::map<std::string, unsigned int> Loader::cachedTextures = {};

Loader::Loader()
{
}


Loader::~Loader()
{
}

// Loads a .TGA image file from a given file path and generates a GL_TEXTURE_2D, returning a GLuint/unsigned int for it.
void Loader::loadTGA(const std::string& filePath, unsigned int& textureID)
{

	if (cachedTextures.find(filePath) != cachedTextures.end())
	{
		textureID = cachedTextures[filePath];
		return;
	}

	std::ifstream fileStream(filePath, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "[ERROR] Failed to load TGA file at " << filePath << " ! Invalid directory?" << std::endl;
		return;
	}

	GLubyte		header[18];									// first 6 useful header bytes
	GLuint		bytesPerPixel;									// number of bytes per pixel in TGA gile
	GLuint		imageSize;										// for setting memory
	GLubyte*	data;
	unsigned	width, height;

	fileStream.read((char*)header, 18);
	width = header[12] + header[13] * 256;
	height = header[14] + header[15] * 256;

	// Wrong bit compression format or invalid file
	if (width <= 0 || height <= 0 || (header[16] != 24 && header[16] != 32))
	{
		fileStream.close();							
		std::cout << "[ERROR] Failed to load TGA file at " << filePath << " ! Corrupted file?" << std::endl;
		return;
	}

	// Divide by 8 to get bytes per pixel
	bytesPerPixel = header[16] / 8;
	// Calculate memory required for TGA data
	imageSize = width * height * bytesPerPixel;	

	data = new GLubyte[imageSize];
	fileStream.seekg(18, std::ios::beg);
	fileStream.read((char *)data, imageSize);
	fileStream.close();

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Read alpha values if supported
	if (bytesPerPixel == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);


	// Texture Settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Auto generate Mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	float maxAntisotropy = 1.f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAntisotropy);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)maxAntisotropy);

	cachedTextures[filePath] = textureID;

	delete[] data;
}

// Loads a .OBJ file from a given file path and generates an OBJInfo object containing all Vertex (position,uv,normals) and Indices information.
void Loader::loadOBJ(const std::string& filePath, OBJInfo& outInfo)
{
	std::ifstream handle(filePath);
	if (!handle.is_open())
	{
		std::cout << "[ERROR] Could not load OBJ file at " << filePath << " ! Invalid directory?" << std::endl;
	}


	// Input
	std::string line;
	std::vector<Vector2> texCoords;
	std::vector<Vector3> normals;
	std::vector<Vector3> positions;

	// Stores the index of each vertex (position, texCoord and normal) of each triangle 
	// Index is the position of the specific attribute in its respective vector, but incremented by 1 
	// since vector starts from 0 while obj file format starts from 1
	std::vector<unsigned int> vertIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;


	while (!handle.eof()){
		char buffer[256];
		handle.getline(buffer, 256);
		Vector3 v;

		// Vertex
		if (strncmp("v ", buffer, 2) == 0)
		{
			sscanf_s((buffer + 2), "%f%f%f", &v.x, &v.y, &v.z);
			positions.push_back(v);
		}

		// Texture Coordinate
		else if (strncmp("vt ", buffer, 3) == 0)
		{
			sscanf_s((buffer + 3), "%f%f", &v.x, &v.y);
			texCoords.emplace_back(v.x, v.y);
		}

		// Vertex Normal
		else if (strncmp("vn ", buffer, 3) == 0)
		{
			sscanf_s((buffer + 3), "%f%f%f", &v.x, &v.y, &v.z);
			normals.push_back(v);
		}

		// Face
		else if (strncmp("f ", buffer, 2) == 0)
		{
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];

			int matches = sscanf_s((buffer + 2), "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2],
				&vertexIndex[3], &uvIndex[3], &normalIndex[3]);

			if (matches == 9 || matches == 12)
			{
				// Triangle & Quad
				for (int i = 0; i < 3; i++)
				{
					vertIndices.push_back(vertexIndex[i]);
					uvIndices.push_back(uvIndex[i]);
					normalIndices.push_back(normalIndex[i]);
				}

				// Quad
				if (matches == 12) {
					vertIndices.push_back(vertexIndex[2]);
					vertIndices.push_back(vertexIndex[3]);
					vertIndices.push_back(vertexIndex[0]);
					uvIndices.push_back(uvIndex[2]);
					uvIndices.push_back(uvIndex[3]);
					uvIndices.push_back(uvIndex[0]);
					normalIndices.push_back(normalIndex[2]);
					normalIndices.push_back(normalIndex[3]);
					normalIndices.push_back(normalIndex[0]);
				}
			}
			else
			{
				// N-sided polygons
				std::cout << "[ERROR] This model contains " << matches << "-sided polygons! (" << filePath << ")" << std::endl;
				break;
			}
		}
	}

	handle.close();


	// Output
	std::map<Vertex, unsigned int> data;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	// Loop through all the indices
	for (unsigned int i = 0; i < (unsigned int)vertIndices.size(); i++)
	{
		unsigned int vertIndex = vertIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normIndex = normalIndices[i];

		// Index starts from 1 in obj file
		// Therefore, actual position in vector = Index - 1
		Vector3 pos = positions[vertIndex - 1];
		Vector2 uv = texCoords[uvIndex - 1];
		Vector3 norm = normals[normIndex - 1];

		Vertex vertex(pos, uv, norm);

		// Add the same index number if vertex already exists
		if (data.find(vertex) != data.end())
		{
			indices.emplace_back(data.at(vertex));
		}
		else
		{
			// If vertex, does not exist in collection yet, add it
			// and the corresponding index.
			vertices.push_back(vertex);
			unsigned int newIndex = ((unsigned int)vertices.size()) - 1;
			indices.push_back(newIndex);
			data[vertex] = newIndex;
		}
	}

	data.clear();

	outInfo.vertices = std::move(vertices);
	outInfo.indices = std::move(indices);
}


