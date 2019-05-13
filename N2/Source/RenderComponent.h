#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H


#include "Component.h"
#include "Info.h"
#include "ShaderProgram.h"
#include <GL\glew.h>

class Entity;

struct Material {

	Material(Vector3 ambient, Vector3 diffuse, Vector3 specular, float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
	Material() {}

	bool operator==(const Material& other)
	{
		return (ambient == other.ambient) && (diffuse == other.diffuse) && (specular == other.specular) && (shininess == other.shininess);
	}

	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	float shininess;
};


enum DRAW_MODE
{
	DRAW_TRIANGLES = 0,
	DRAW_TRIANGLE_STRIP,
	DRAW_LINES,
	DRAW_FAN,
	DRAW_MODE_LAST,
};

class RenderComponent : public Component
{
public:

	RenderComponent(Entity* parent, ShaderProgram* shader, std::string texturePath, DRAW_MODE mode = DRAW_TRIANGLES);
	RenderComponent(Entity* parent, ShaderProgram* shader, std::string modelPath, std::string texturePath, DRAW_MODE mode = DRAW_TRIANGLES);
	
	RenderComponent(Entity* parent, ShaderProgram* shader, bool doubleSided, std::string modelPath, std::string texturePath, DRAW_MODE mode = DRAW_TRIANGLES);
	
	RenderComponent(Entity* parent, ShaderProgram* shader);
	RenderComponent();
	~RenderComponent();

	void setInfo(const OBJInfo& info);
	void setMaterial(const Material& material);

	/* Needed for Rendering */
	const OBJInfo& getInfo() const;
	const DRAW_MODE& getMode() const;
	const Material& getMaterial() const;
	const unsigned int& getTexID() const;
	ShaderProgram* getShader() const;
	const bool& isDoubleSided() const;

private:
	DRAW_MODE mode;
	Material material;
	OBJInfo info;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int textureID;
	ShaderProgram* shader;
	bool doubleSided;
};

#endif