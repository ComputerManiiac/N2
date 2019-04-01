#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>



class FrameBuffer
{
public:
	FrameBuffer(unsigned int width, unsigned int height, GLenum mode);
	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void Unbind();

	const unsigned int& getTexID() const;
	const unsigned int& getID() const;

private:
	GLenum mode;
	unsigned int width;
	unsigned int height;
	unsigned int textureID;
	unsigned int id;
};

#endif