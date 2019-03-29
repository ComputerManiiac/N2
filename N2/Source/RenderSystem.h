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
#include <vector>
#include <algorithm>


struct VertexData {
	
	VertexData(Mtx44 model, Mtx44 view, Mtx44 projection) : model(model), view(view), projection(projection) {}
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
};

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();

	void Initialize();
	void initializeComponent(const OBJInfo& info, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
	void setupLight();

	void Update(double& dt);
	void updateTransformMatrices();
	void layoutInstancedData(const unsigned int& VAO);
	

	void registerComp(Component* component);
	void removeComp(Component* component);


private:
	ShaderProgram* lit;

	unsigned int matricesVBO;
	std::vector<VertexData> data;


	std::vector<RenderComponent*> subscribers;
	std::vector<LightSource*> lightSources;
	MS modelStack, projectionStack, viewStack;

};

#endif