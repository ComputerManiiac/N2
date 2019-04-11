#ifndef LOADER_H
#define LOADER_H


#include <GL\glew.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include "Info.h"
#include "Text.h"
#include "OverloadNew.h"

/*

[Utility] - Loader

-> Purpose: In charge of loading, serializing and saving of data in the form of images, obj or scene files.
-> Methods:

	-> loadTGA(): Loads a .TGA image file from a given file path and generates a GL_TEXTURE_2D, returning a GLuint/unsigned int for it.
	-> loadOBJ(): Loads a .OBJ file from a given file path and generates an OBJInfo object containing all Vertex (position,uv,normals) and Indices information.

*/






class Loader
{
public:
	Loader();
	~Loader();
	static void loadCubemap(const std::vector<std::string>& filePath, unsigned int& textureID, GLint filterMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);
	static void loadTGA(const std::string& filePath, unsigned int& textureID, GLint filterMode=GL_LINEAR, GLint wrapMode=GL_CLAMP_TO_EDGE);
	static void loadOBJ(const std::string& filePath, OBJInfo& outInfo);
	static void loadFont(const std::string& filePath, Font& font);

	static std::map<std::string, unsigned int>& getCachedTextures();

private:

	static void loadImg(const std::string& filePath, GLubyte*& data, GLuint& bytesPerPixel, unsigned int& width, unsigned int& height);

	static float getValueFloat(const std::string& line);
	static int getValueInt(const std::string& line);

	static std::map<std::string, OBJInfo> cachedInfo;
	static std::map<std::string, unsigned int> cachedTextures;
};

#endif