#ifndef BATCH_H
#define BATCH_H

#include "Mtx44.h"
#include "ShaderProgram.h"
#include "RenderComponent.h"
#include <vector>

struct VertexData {

	VertexData(Mtx44 model): model(model){}
	//VertexData(Mtx44 model, Mtx44 view, Mtx44 projection) : model(model), view(view), projection(projection) {}
	Mtx44 model;
	//Mtx44 view;
	//Mtx44 projection;
};


struct BatchKey {


	BatchKey(ShaderProgram* shader, unsigned int textureID, Material mat) : shader(shader), textureID(textureID), mat(mat) {}
	BatchKey() {}


	bool operator==(const BatchKey& other)
	{
		return (other.textureID == textureID) && (mat == other.mat) && (shader == other.shader);
	}

	bool operator < (const BatchKey other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(BatchKey)) > 0;
	}

	void setAll(ShaderProgram* shader, const unsigned int& textureID, const Material& material)
	{
		this->shader = shader;
		this->textureID = textureID;
		this->mat = material;
	}

	unsigned int textureID;
	Material mat;
	ShaderProgram* shader;
};


struct Batch {

	Batch(std::vector<RenderComponent*> subscribers) : subscribers(subscribers) {}
	Batch() : VAO(0), VBO(0), EBO(0) {}


	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	std::vector<VertexData> data;
	std::vector<RenderComponent*> subscribers;
	OBJInfo info;
};

#endif