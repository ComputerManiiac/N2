#ifndef GAMEAPPLICATION_H
#define GAMEAPPLICATION_H

#include "Application.h"

class GameApplication : public Application
{
public:
	GameApplication(std::string title, unsigned int screenWidth, unsigned int screenHeight);
	GameApplication() = delete;
	~GameApplication();

	void Run();
};

#endif