#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Vector3.h"
#include "Mtx44.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


class ShaderProgram
{
public:

	ShaderProgram() {}
	ShaderProgram(std::string vertexPath, std::string fragmentPath);

	void Use();
	void setUniform(const char* name, const bool& value);
	void setUniform(const char* name, const int& value);
	void setUniform(const char* name, const float& value);
	void setUniform(const char* name, const float& x, const float& y, const float& z);
	void setUniform(const char* name, const Vector3& vec);
	void setUniform(const char* name, const float& x, const float& y, const float& z, const float& w);
	void setUniform(const char* name, const Mtx44& matrix);

	const std::string& getVertexPath() const;
	const std::string& getFragmentPath() const;


private:


	unsigned int loadAndCompile(unsigned int type, const std::string &filePath);
	unsigned int id;
	std::string vertexPath = "";
	std::string fragmentPath = "";
	
};

#endif





