/**
	@file engine-main.cpp
	@author Tarkan Kemalzade
	@date 04/02/2017
	@version 0.0.0.0
*/

#include <iostream>
#include <string>
#include <Graphics-Engine\window-manager.h>

int main()
{
	
	std::cout << "Engine Name: Dark Nebula" << std::endl;
	std::cout << "Engine Version: 0.0.0.0" << std::endl;

	WindowManager app(500, 500, "Dark Nebula");
	app.initialiseGL();
	app.mainLoop();

	return 0;
}