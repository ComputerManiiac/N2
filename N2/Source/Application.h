#ifndef APPLICATION_H
#define APPLICATION_H

#include "OverloadNew.h"
#include "timer.h"
#include "Manager.h"
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

	virtual void Run();

	static Manager* getManager();

	static void toggleCursorLock(int state);

	static float getMouseScrollDelta();

	static bool isKeyPressed(int key);
	static bool isKeyPressDown(int key);
	static bool isKeyPressRelease(int key);

	static unsigned int getScreenWidth();
	static unsigned int getScreenHeight();

	static unsigned int getHalfScreenWidth();
	static unsigned int getHalfScreenHeight();

	static double getMouseX();
	static double getMouseY();

	void setManager(Manager* manager);

	static int framesPerSecond;

private:

	static void keyCallback(GLFWwindow* window, int key, int scanNode, int action, int mods);
	static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);



	std::string title;

protected:


	static Manager* manager;

	static std::map<int, bool> keyDown;
	static std::map<int, bool> keyRelease;

	static double mouseX;
	static double mouseY;

	static float mouseScrollDeltaY;

	static unsigned int screenWidth;
	static unsigned int screenHeight;

	static unsigned int halfScreenWidth;
	static unsigned int halfScreenHeight;

	static GLFWwindow* window;

};

#endif