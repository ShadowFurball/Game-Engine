/**
	@headerfile window-manager.h
	@author Tarkan Kemalzade
	@date 04/02/2017
*/

#pragma once

#ifndef _WINDOW_MANAGER_H
#define _WINDOW_MANAGER_H

#include <string>
#include <gl_core_4_3.hpp>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

class WindowManager
{
	public:
		WindowManager(int, int, std::string);
		WindowManager(std::string, bool);
		~WindowManager();

		static void error_callback(int, const char*);
		static void framebuffer_callback(GLFWwindow*, int, int);
		
		bool initialiseWindow();
		void destroyWindow();
		void initialiseGL();
		void mainLoop();
		void update(float);



	private:
		GLFWwindow* m_pWindow; //! Member Variable: GLFW window object (pointer).
		int m_width; //! Member Variable: window object width.
		int m_height; //! Member Variable: window object height.
		std::string m_windowID; //! Member Variable: ID / Name of window object.
		bool m_fullScreenEnabled; //! Member Varaibles: Checks window object for full screen
		glm::dvec2 currentCursorPosition;
		glm::dvec2 lastCursorPosition;
};
#endif // !_WINDOW_MANAGER_H
