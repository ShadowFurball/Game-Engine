/**
	@file window-manager.cpp
	@author Tarkan Kemalzade
	@date 04/02/2017
*/

#include <Graphics-Engine\window-manager.h>


Scene *scene;
Camera camera;

/**
	Contructs a window manager object
	@param width - interger
	@param height - interger
	@param title - string
	@fullScreen - boolean
*/
WindowManager::WindowManager(int width, int height, std::string title)
{
	m_width = width;
	m_height = height;
	m_windowID = title;
	m_fullScreenEnabled = false;

	initialiseWindow();
}

/**
Contructs a window manager object
@param title - string
@fullScreen - boolean
*/
WindowManager::WindowManager(std::string title, bool fullScreenMode)
{
	m_width = 1920;
	m_height = 1080;
	m_windowID = title;
	m_fullScreenEnabled = fullScreenMode;

	initialiseWindow();
}

/**
	Default destructor.
*/
WindowManager::~WindowManager()
{

}

/**
	Initialises the GLFW window
	@return 1
*/
bool WindowManager::initialiseWindow()
{
	if (!glfwInit()) // exit application,
	{
		exit(EXIT_FAILURE);
	}

	// select OpenGL 4.3 forward compatible, core and profile,
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, TRUE);


	if (m_fullScreenEnabled) // create window in full screen mode,
	{
		m_pWindow = glfwCreateWindow(m_width, m_height, m_windowID.c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else // create window as a windowed screen,
	{
		m_pWindow = glfwCreateWindow(m_width, m_height, m_windowID.c_str(), NULL, NULL);
	}

	if (!m_pWindow) // terminate the window
	{
		destroyWindow();
	}

	// create the current context
	glfwMakeContextCurrent(m_pWindow);

	// set callbacks
	glfwSetErrorCallback(error_callback);

	// loading function pointers
	gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
	if (!didLoad)
	{
		destroyWindow();
	}

	return 1;
}

/**
	Destroys the current window
*/
void WindowManager::destroyWindow()
{
	glfwTerminate();
	exit(-1);
}

/**
	Initialises any open gl code.
*/
void WindowManager::initialiseGL()
{
	gl::ClearColor(0.f, 0.4f, 0.9f, 0.5f);

	currentCursorPosition = glm::dvec2(0, 0);
	lastCursorPosition = glm::dvec2(0, 0);

    scene = new EngineScene();
    scene->initScene(camera);
}

/**
	Outputs the window manager error onto the console.
	@param error - error code
	@param description
*/
void WindowManager::error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

/**
    Callback function used to toggle animation and to use R to reset the camera position.

	@param window
	@param key
	@param cancode -  size of the key code
	@param action- Action for whether the key was released or not.
	@param mods
*/
void WindowManager::key_callback(GLFWwindow* window, int key, int cancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    {
        if (scene)
        {
            scene->animate(!(scene->animating()));
        }
    }
}

/**
	Framebuffer callback
	@param window - current window
	@param width
	@param height
*/
void WindowManager::framebuffer_callback(GLFWwindow* window, int width, int height)
{
	glfwGetFramebufferSize(window, &width, &height);
}

/**
	Updates the current window
*/
void WindowManager::update(float time) 
{
	// Gets the current cursor positions.
	glfwGetCursorPos(m_pWindow, &currentCursorPosition.x, &currentCursorPosition.y);
	
	// Gets how much the cursor has moved from its last position.
	double delatCursorX = lastCursorPosition.x - currentCursorPosition.x;
	double deltaCursorY = lastCursorPosition.y - currentCursorPosition.y;

	// Store the current cursor position into the last position.
	lastCursorPosition = currentCursorPosition;

	// Check for any resizing events
	glfwSetFramebufferSizeCallback(m_pWindow, framebuffer_callback);

}

void WindowManager::mainLoop()
{
	while (!glfwWindowShouldClose(m_pWindow) && !glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE))
	{
		update((float)glfwGetTime());
        scene->render(camera);

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
}