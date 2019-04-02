#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

/*

[System] - Render System

-> Purpose: In charge of rendering in-game and on-screen objects, which includes UI elements as well as models in game.
-> Components Used: RenderComponent, ColliderComponent* (* - optional)

   -> RenderComponent: The main component used for rendering, contains data such as VAO, VBO, EBO and draw mode.
   -> ColliderComponent: Used to render collision bounds, contains data about an object's collision box.

-> Methods:

	-> Initialize():
		-> Defines projection matrix for the scene
		-> Generates vertex arrays and buffers for each render component and populates the assigned int into them
		-> Sets up how attributes are layed out in the VAO before buffering data into VBO
		-> Sets up values for all the lights in the Scene
		-> [NOTE] Render Components are required to be subscribed to Render System before this method is called for actual buffers to be generated!

	-> Update():
		->  Uses data that are stored in the RenderComponent (stored previously through Initialize) to draw objects

	-> updateTransform():
		-> Apply an object's transformations to the model stack

	-> updateShader():
		-> Update an object's transformational related information, as well as material and texture for lighting.

	-> drawCollider():
		-> Draws an object's box collider
*/




#include "System.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "MatrixStack.h"
#include "LightSource.h"
#include "FrameBuffer.h"
#include <vector>
#include <algorithm>
#include <string>

struct VertexData {

	VertexData(Mtx44 model, Mtx44 view, Mtx44 projection) : model(model), view(view), projection(projection) {}
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
};

struct BatchKey {


	BatchKey(unsigned int textureID, Material mat) : textureID(textureID), mat(mat) {}
	BatchKey() {}


	bool operator==(const BatchKey& other)
	{
		return (other.textureID == textureID) && (mat == other.mat);
	}

	bool operator < (const BatchKey other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(BatchKey)) > 0;
	}

	unsigned int textureID;
	Material mat;
};


struct Batch {

	Batch(std::vector<RenderComponent*> subscribers) : subscribers(subscribers) {}
	Batch() {}

	std::vector<VertexData> data;
	std::vector<RenderComponent*> subscribers;
};

enum TextAlignment {
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_RIGHT,
};



class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();

	void Initialize();

	void setupComponent(const OBJInfo& info, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
	void setupLight();
	void setupShadows();

	void Update(double& dt);

	void renderScene(ShaderProgram* shader, const Mtx44& viewMatrix);


	/* Temp function to debug framebuffer's texture */
	void renderTexture(const FrameBuffer& buffer);
	

	void updateTransformMatrices(Batch& batch);
	void layoutInstancedData(const unsigned int& VAO);
	

	void registerComp(Component* component);
	void removeComp(Component* component);


	void renderText(const std::string& text, float xPos, float yPos, Font& font, float fontSize = 1.0f, TextAlignment align = TEXT_ALIGN_LEFT);

private:

	ShaderProgram* depth;
	ShaderProgram* lit;
	ShaderProgram* ui;
	
	/* Used for rendering textures to screen */
	unsigned int quadVAO;
	unsigned int quadVBO;
	unsigned int quadEBO;
	
	/* Shadows */
	FrameBuffer shadowFBO;	
	Mtx44 lightProjectionView, lightProjection, lightView;

	unsigned int batchVBO;
	std::map<BatchKey, Batch> batches;

	std::vector<Text> texts;
	std::vector<RenderComponent*> subscribers;
	std::vector<LightSource*> lightSources;
	std::vector<Font> fonts;
	MS modelStack;
	Mtx44 projection, view;


};

#endif