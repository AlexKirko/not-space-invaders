// main gaming loop implementation

#include "RenderedObject.h"

// GLEW: for loading OpenGL functions newer than OpenGL 1.1
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW: OpenGL framework - cross-platform window
// and access to context
#include <GLFW/glfw3.h>

#include <iostream>


void error_callback(int error_code, const char* description)
{
	std::cerr << "Error: " << description;
}

int main()
{
	// Window size
	const GLint WIDTH{ 800 };
	const GLint HEIGHT{ 600 };

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		return EXIT_FAILURE;
	}

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Space Invaders", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cerr << "Error creating OpenGL window.";

		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Get frame buffer size
	int window_width{ 0 };
	int window_height{ 0 };

	glfwGetFramebufferSize(window, &window_width, &window_height);

	glfwMakeContextCurrent(window);

	// GLEW requires a current context, so we initialize it here
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Error initializing GLEW.";

		return EXIT_FAILURE;
	}

	std::cout << glGetString(GL_VERSION) << '\n';

	// Set up the viewport
	glViewport(0, 0, window_width, window_height);

	// Start the main loop
	while (!glfwWindowShouldClose(window))
	{
		// Render
		glClearColor(0.0, 0.5, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Show rendered window
		glfwSwapBuffers(window);

		// Poll waiting events, send them to callbacks,
		// return whether there are events or not
		glfwPollEvents();
	}

	glfwTerminate();

	std::cout << "Execution successful.";

	return EXIT_SUCCESS;
}


