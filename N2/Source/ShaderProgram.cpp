#include "ShaderProgram.h"



ShaderProgram::ShaderProgram(std::string vertexPath, std::string fragmentPath)
{

	this->vertexPath = vertexPath;
	this->fragmentPath = fragmentPath;

	id = glCreateProgram();
	unsigned int vShader = loadAndCompile(GL_VERTEX_SHADER, vertexPath);
	unsigned int fShader = loadAndCompile(GL_FRAGMENT_SHADER, fragmentPath);
	

	glAttachShader(id, vShader);
	glAttachShader(id, fShader);
	glLinkProgram(id);
	glValidateProgram(id);
	glDeleteShader(vShader);
	glDeleteShader(fShader);

}


ShaderProgram::ShaderProgram(std::string vertexPath, std::string geometryPath, std::string fragmentPath)
{

	this->vertexPath = vertexPath;
	this->geometryPath = geometryPath;
	this->fragmentPath = fragmentPath;

	id = glCreateProgram();
	unsigned int vShader = loadAndCompile(GL_VERTEX_SHADER, vertexPath);
	unsigned int gShader = loadAndCompile(GL_GEOMETRY_SHADER, geometryPath);
	unsigned int fShader = loadAndCompile(GL_FRAGMENT_SHADER, fragmentPath);
	
	glAttachShader(id, vShader);
	glAttachShader(id, fShader);
	glAttachShader(id, gShader);
	glLinkProgram(id);
	glValidateProgram(id);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	glDeleteShader(gShader);

}

void ShaderProgram::Use()
{
	glUseProgram(id);
}

unsigned int ShaderProgram::loadAndCompile(unsigned int type, const std::string &filePath)
{
	std::ifstream handle(filePath);
	std::string line = "";
	std::string source = "";

	if (!handle.is_open())
		std::cout << "[Error] Could not load shader: " << filePath << std::endl;

	while (std::getline(handle, line))
		source += line + "\n";

	handle.close();

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	// Prompt error message in Console
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "[Error] Could not compile " << ((type == GL_VERTEX_SHADER) ? "Vertex shader!" : "Fragment shader!") << " (" << filePath << ")" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void ShaderProgram::setUniform(const char* name, const bool& value)
{
	glUniform1i(glGetUniformLocation(id, name), (int)value);
}
void ShaderProgram::setUniform(const char* name, const int& value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}
void ShaderProgram::setUniform(const char* name, const float& value)
{
	glUniform1f(glGetUniformLocation(id, name), value);
}

void ShaderProgram::setUniform(const char* name, const Vector3& vec)
{
	setUniform(name, vec.x, vec.y, vec.z);
}

void ShaderProgram::setUniform(const char* name, const float& x, const float& y, const float& z)
{
	glUniform3f(glGetUniformLocation(id, name), x, y, z);
}

void ShaderProgram::setUniform(const char* name, const float& x, const float& y, const float& z, const float& w)
{
	int attrid = glGetUniformLocation(id, name);
	glUniform4f(attrid, x, y, z, w);
}

void ShaderProgram::setUniform(const char* name, const Mtx44& matrix)
{
	unsigned int attrid = glGetUniformLocation(id, name);
	glUniformMatrix4fv(attrid, 1, false, &matrix.a[0]);
}

const std::string& ShaderProgram::getVertexPath() const
{
	return vertexPath;
}

const std::string& ShaderProgram::getFragmentPath() const
{
	return fragmentPath;
}

const std::string& ShaderProgram::getGeometryPath() const
{
	return geometryPath;
}

