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
	static void loadTGA(const std::string& filePath, unsigned int& textureID);
	static void loadOBJ(const std::string& filePath, OBJInfo& outInfo);
	static void loadFont(const std::string& filePath, Font& font);
private:
	static std::map<std::string, unsigned int> cachedTextures;
	static float getValueFloat(const std::string& line);
	static int getValueInt(const std::string& line);

};

#endif