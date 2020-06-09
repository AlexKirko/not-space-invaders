// main gaming loop implementation

// Define if we need to force program to run on an Nvidia GPU
#define NVIDIA_GPU
#define DEBUG

#include "opengl_debug_output.h"

#include "Renderer.h"

#include "RenderedObject.h"
#include "Battlefield.h"

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

static void error_callback(int error_code, const char* description)
{
	std::cerr << "Error: " << description;
}

static inline void set_window_properties()
{
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
}

static inline void setup_debug_callback()
{
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
}

// Setup OpenGL window, context, error handling, etc.
static GLFWwindow* opengl_setup(int in_window_width, int in_window_height,
	int& out_window_width, int& out_window_height,
	const std::string& window_name)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		throw std::runtime_error("GLFW failed to initialize.");
	}

	set_window_properties();

	GLFWwindow* window = glfwCreateWindow(in_window_width, in_window_height,
		window_name.c_str(), nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window.");
	}

	// Get frame buffer size
	glfwGetFramebufferSize(window, &out_window_width, &out_window_height);

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	// GLEW requires a current context, so we initialize it here
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("Failed to initialize GLEW.");
	}

	setup_debug_callback();

	//Display OpenGL version
	std::cout << glGetString(GL_VERSION) << '\n';

	// Set up the viewport
	glViewport(0, 0, out_window_width, out_window_height);

	// Set the blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);


	return window;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Battlefield* battlefield = static_cast<Battlefield*>(glfwGetWindowUserPointer(window));

	// Player movement
	bool movement{ false };
	MoveTypes player_move{ MoveTypes::STILL };
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		movement = true;
		player_move = MoveTypes::RIGHT;
	}
	else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		movement = true;
		player_move = MoveTypes::LEFT;
	}
	else if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT) && action == GLFW_RELEASE)
	{
		movement = true;
	}
	if (movement)
		battlefield->get_player()->movement(player_move, battlefield->get_player_speed());

	// Player shooting
	if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		battlefield->get_player()->push_bullets(1.5f, 200.0f);
	}
}

int main()
{
	// Window size
	const GLint WIDTH{ 800 };
	const GLint HEIGHT{ 600 };
	int window_width{ 0 };
	int window_height{ 0 };

	GLFWwindow* window = opengl_setup(WIDTH, HEIGHT,
		window_width, window_height, "Space Invaders");

	glfwSetKeyCallback(window, key_callback);

	Renderer renderer{};

	Battlefield battlefield{ static_cast<float>(window_width), static_cast<float>(window_height) };
	glfwSetWindowUserPointer(window, &battlefield);
	// Spawn the player (spawn coordinates calculated in the Battlefield class)
	battlefield.create_player();
	// Start the main loop
	double last_loop_time{ glfwGetTime() };
	double last_move_time{ glfwGetTime() };
	float time_elapsed{};
	while (!glfwWindowShouldClose(window))
	{
		// Render
		renderer.clear(std::array<float, 4> {0.0f, 0.0f, 0.0f, 1.0f});

		last_loop_time = glfwGetTime();

		battlefield.spawn_alien_row(21, 10.0);
		//if (glfwGetTime() - last_move_time > 0.05)
		//{
		//}
		time_elapsed = static_cast<float>(glfwGetTime() - last_move_time);
		last_move_time = glfwGetTime();

		// Ticking actions
		battlefield.aliens_shoot(time_elapsed);

		battlefield.get_player()->tick_time(time_elapsed);
		battlefield.player_shoots();

		battlefield.move_objects(time_elapsed);

		battlefield.check_hits();

		battlefield.render_objects();

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
