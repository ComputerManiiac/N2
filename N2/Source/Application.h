#ifndef APPLICATION_H
#define APPLICATION_H

#include "OverloadNew.h"
#include "timer.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <map>



class Application
{
public:
	
	Application(std::string title, unsigned int screenWidth, unsigned int screenHeight);
	Application();
	~Application();

	void Run();

	static bool isKeyPressed(int key);
	static bool isKeyPressDown(int key);
	static bool isKeyPressRelease(int key);

	static unsigned int getScreenWidth();
	static unsigned int getScreenHeight();

	static double getMouseX();
	static double getMouseY();

private:

	static void keyCallback(GLFWwindow* window, int key, int scanNode, int action, int mods);
	static void mouseCallback(GLFWwindow* window, double xPos, double yPos);

	static std::map<int, bool> keyDown;
	static std::map<int, bool> keyRelease;

	static double mouseX;
	static double mouseY;
	static unsigned int screenWidth;
	static unsigned int screenHeight;
	static GLFWwindow* window;

	std::string title;



};

#endif