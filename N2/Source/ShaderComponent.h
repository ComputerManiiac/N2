#ifndef SHADERCOMPONENT_H
#define SHADERCOMPONENT_H

#include "Component.h"
#include "ShaderProgram.h"

class ShaderComponent : public Component
{
public:

	ShaderComponent(ShaderProgram* shader);
	ShaderComponent();
	~ShaderComponent();

	const ShaderProgram* getShader() const;

private:
	ShaderProgram* shader;
};

#endif