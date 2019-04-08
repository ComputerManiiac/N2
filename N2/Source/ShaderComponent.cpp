#include "ShaderComponent.h"


ShaderComponent::ShaderComponent(ShaderProgram* shader) : shader(shader)
{

}


ShaderComponent::ShaderComponent() : shader(nullptr)
{
	
}


ShaderComponent::~ShaderComponent()
{
}


const ShaderProgram* ShaderComponent::getShader() const
{
	return shader;
}