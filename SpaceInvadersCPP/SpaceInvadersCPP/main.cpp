// main gaming loop implementation

// Define if we need to force program to run on an Nvidia GPU
#define NVIDIA_GPU
#define DEBUG

#include "opengl_debug_output.h"

#include "Renderer.h"

#include "RenderedObject.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// GLEW: for loading OpenGL functions newer than OpenGL 1.1
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW: OpenGL framework - cross-platform window
// and access to context
#include <GLFW/glfw3.h>

#include <array>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <vector>

// Necessary to force program running on Nvidia
#ifdef NVIDIA_GPU
#include <Windows.h>

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

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

	glfwSwapInterval(1);

	// GLEW requires a current context, so we initialize it here
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Error initializing GLEW.";

		return EXIT_FAILURE;
	}

	// If successfuly in debug, register callback and init
	int gl_context_flags{};
	glGetIntegerv(GL_CONTEXT_FLAGS, &gl_context_flags);
	if (gl_context_flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// The only place we need old-fashioned OpenGL error-handling
		// is here
		while (glGetError() != GL_NO_ERROR);
		glDebugMessageCallback(opengl_debug_output, nullptr);
		unsigned int error{};
		while ((error = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << "Error code: " << error << '\n';
		}
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	std::cout << glGetString(GL_VERSION) << '\n';

	// Set up the viewport
	glViewport(0, 0, window_width, window_height);

	// Set the blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	

	Texture texture{ "res/textures/cpp_logo_200.png" };
	texture.bind();
	auto ro_tex = std::make_shared<std::vector<Texture>>();
	ro_tex->push_back(texture);
	texture.unbind();

	// Create a sample RenderedObject
	RenderedObject rObj(
		std::array<float, 2>{100.0, 50.0},
		50, 50,
		true, true,
		0.0,
		std::array<float, 2>{0.0, 0.0},
		std::array<float, 4>{1.0, 1.0, 1.0, 1.0},
		ro_tex
	);
	rObj.set_current_texture(0);

	// Make an array of objects by copying
	std::vector<RenderedObject> rObj_v{};
	for (int i{ 0 }; i < 10; ++i)
	{
		for (int k{ 0 }; k < 5; ++k)
		{
			RenderedObject rObj_push(rObj);
			rObj_push.move_to(std::array<float, 2>{
				rObj.get_bottomleft()[0] + i * 60,
				rObj.get_bottomleft()[0] + k * 60,
			});
			rObj_v.push_back(rObj_push);
		}
	}

	Renderer renderer{};

	for (auto rObject : rObj_v)
	{
		renderer.init_render(rObject, WIDTH, HEIGHT);
	}

	// Start the main loop
	while (!glfwWindowShouldClose(window))
	{
		// Render
		renderer.clear(std::array<float, 4> {0.0f, 0.0f, 0.0f, 1.0f});

		for (auto rObject : rObj_v)
		{
			renderer.render(rObject, WIDTH, HEIGHT);
		}


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
