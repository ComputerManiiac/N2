#include "GameApplication.h"




GameApplication::GameApplication(std::string title, unsigned int screenWidth, unsigned int screenHeight) :Application(title, screenWidth, screenHeight)
{	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GameApplication::~GameApplication()
{
}

void GameApplication::Run()
{

	manager->Initialize();
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
