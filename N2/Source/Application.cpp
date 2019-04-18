#include "Application.h"
#include "Manager.h"

GLFWwindow* Application::window = nullptr;
std::map<int, bool> Application::keyDown = {};
std::map<int, bool> Application::keyRelease = {};



double Application::mouseX = 0.0;
double Application::mouseY = 0.0;

float Application::mouseScrollDeltaY = 0.0f;

unsigned int Application::screenWidth = 0;
unsigned int Application::screenHeight = 0;
unsigned int Application::halfScreenWidth = 0;
unsigned int Application::halfScreenHeight = 0;

int Application::framesPerSecond = 0;


Manager* Application::manager = nullptr;


Application::Application(std::string title, unsigned int screenWidth, unsigned int screenHeight) : title(title)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	this->halfScreenWidth = (unsigned int) screenWidth / 2;
	this->halfScreenHeight = (unsigned int) screenHeight / 2;

	/* Initialize the library */
	if (!glfwInit()) return;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(screenWidth, screenHeight, title.c_str(), NULL, NULL);
	if (!window) glfwTerminate();

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "[ERROR] GLEW not initialized properly!" << std::endl;
		return;
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	
}

Application::Application() 
{
}


Application::~Application()
{
}



void Application::Run()
{

	Manager* manager = Manager::getInstance();

	StopWatch timer;
	timer.startTimer();
	timer.getElapsedTime();


	double lastTime = glfwGetTime();
	int frames = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (Application::isKeyPressDown(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}


		manager->Update(timer.getElapsedTime());

		frames++;
		/* FPS Counter */
		if (glfwGetTime() - lastTime >= 1.0f)
		{
			framesPerSecond = frames;
			frames = -1;
			lastTime = glfwGetTime();
		}

		/* Clear all key down and release states */
		keyDown.clear();
		keyRelease.clear();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();

	}

	glfwTerminate();
	delete manager;
}

Manager* Application::getManager()
{
	return manager;
}

void Application::toggleCursorLock(int state)
{
	glfwSetInputMode(window, GLFW_CURSOR, state);
}

float Application::getMouseScrollDelta()
{
	return mouseScrollDeltaY;
}


bool Application::isKeyPressed(int key)
{
	if (key == GLFW_MOUSE_BUTTON_LEFT || key == GLFW_MOUSE_BUTTON_RIGHT)
		return glfwGetMouseButton(window, key);
	else
		return glfwGetKey(window, key);
}

bool Application::isKeyPressDown(int key)
{
	return keyDown.find(key) != keyDown.end();
}

bool Application::isKeyPressRelease(int key)
{
	return keyRelease.find(key) != keyRelease.end();
}

void Application::keyCallback(GLFWwindow* window, int key, int scanNode, int action, int mods)
{

	/* Store key down and key release states of given key */
	if (action == GLFW_PRESS && keyDown.find(key) == keyDown.end())
		keyDown[key] = true;
	
	if (action == GLFW_RELEASE && keyRelease.find(key) == keyRelease.end())
		keyRelease[key] = true;
}

void Application::mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	mouseX = xPos;
	mouseY = yPos;
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	/* Store key down and key release states of given key */
	if (action == GLFW_PRESS && keyDown.find(button) == keyDown.end())
		keyDown[button] = true;

	if (action == GLFW_RELEASE && keyRelease.find(button) == keyRelease.end())
		keyRelease[button] = true;
}

void Application::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	mouseScrollDeltaY = (float) yOffset;
}

void Application::setManager(Manager* manager)
{
	this->manager = manager;
}

unsigned int Application::getScreenWidth()
{
	return screenWidth;
}

unsigned int Application::getScreenHeight()
{
	return screenHeight;
}

unsigned int Application::getHalfScreenWidth()
{
	return halfScreenWidth;
}

unsigned int Application::getHalfScreenHeight()
{
	return halfScreenHeight;
}

double Application::getMouseX()
{
	return mouseX;
}

double Application::getMouseY()
{
	return mouseY;
}
