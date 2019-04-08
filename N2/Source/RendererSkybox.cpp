#include "RendererSkybox.h"
#include "Primitives.h"
#include "Loader.h"

RendererSkybox::RendererSkybox(ShaderProgram* shader) : Renderer(shader, false)
{

}


RendererSkybox::RendererSkybox()
{
}


RendererSkybox::~RendererSkybox()
{
	glDisableVertexAttribArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


void RendererSkybox::Initialize()
{
	shader->Use();
	shader->setUniform("skyboxCubeMap", 0);

	std::vector<Vector3> positions;
	Primitives::generateCubePositions(positions, 1000.0f);

	/* OpenGL Bindings */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vector3), &positions.at(0), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);


	/* Load Textures */
	std::vector<std::string> filePaths = { "Assets\\Textures\\Skybox\\right.tga",
	"Assets\\Textures\\Skybox\\left.tga",
	"Assets\\Textures\\Skybox\\top.tga",
	"Assets\\Textures\\Skybox\\bottom.tga",
	"Assets\\Textures\\Skybox\\front.tga",
	"Assets\\Textures\\Skybox\\back.tga" };

	Loader::loadCubemap(filePaths, textureID, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

void RendererSkybox::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}