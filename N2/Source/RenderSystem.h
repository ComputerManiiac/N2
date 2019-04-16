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
#include "Batch.h"
#include "Text.h"
#include "Renderer.h"
#include "RendererSkybox.h"
#include "RendererParticle.h"
#include <vector>
#include <algorithm>
#include <string>
#include <map>






class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();

	void Initialize();
	void Update(double& dt);

	void registerComp(Component* component);
	void removeComp(Component* component);


	

	void renderText(const std::string& text, float xPos, float yPos, const std::string& fontName, Vector3 color = Vector3(1, 1, 1), float fontSize = 1.0f, TextAlignment align = TEXT_ALIGN_LEFT);

	bool renderSkybox;

	const Mtx44 getProjectionMatrix() const;

private:

	void setupLight();
	void setupShadows();

	void renderScene(const Mtx44& viewMatrix, ShaderProgram* shader=nullptr);
	void renderTexts();

	/* Text Mutation */
	void generateData(const FontChar& fontChar, const float& fontSize, std::vector<UIVertex>& vertices, std::vector<unsigned int>& indices, Vector2& cursor);
	void modifyData(const FontChar& fontChar, const int& i, const float& fontSize, std::vector<UIVertex>& vertices, std::vector<unsigned int>& indices, Vector2& cursor);


	void updateBatchedData();

	/* Temp function to debug framebuffer's texture */
	void renderTexture(const FrameBuffer& buffer);


	/* Shaders */
	ShaderProgram* depth;
	ShaderProgram* lit;
	ShaderProgram* ui;
	
	
	RendererParticle* particle;

	/* Skybox */
	RendererSkybox* skybox;
	
	/* Used for rendering textures to screen */
	unsigned int quadVAO;
	unsigned int quadVBO;
	unsigned int quadEBO;

	/* Text */
	std::map<std::string, Font> fonts;
	std::map<Font*, std::vector<Text*>> texts;
	std::map<unsigned int, Text> mutatedCache;
	unsigned int textUID;

	/* Shadows */
	FrameBuffer shadowFBO;	
	Mtx44 lightProjectionView, lightProjection, lightView;

	/* Batching */
	std::map<BatchKey, Batch> batches;

	std::map<ShaderProgram*, Renderer*> renderers;
	std::vector<RenderComponent*> subscribers;
	std::vector<LightSource*> lightSources;

	MS modelStack;
	Mtx44 projection, view;


};



#endif