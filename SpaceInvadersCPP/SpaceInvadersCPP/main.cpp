// main gaming loop implementation

// Define if we need to force program to run on an Nvidia GPU
#define NVIDIA_GPU
#define DEBUG

#include "opengl_debug_output.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "RenderedObject.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// GLEW: for loading OpenGL functions newer than OpenGL 1.1
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW: OpenGL framework - cross-platform window
// and access to context
#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

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

static unsigned int compile_shader(unsigned int type, const std::string& source)
{
	unsigned int id{ glCreateShader(type) };
	const char* src{ source.c_str() };
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result{};
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length{ };
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cerr << (type == GL_VERTEX_SHADER ? "Vertex " : "Fragment ")
			<< "shader failed to compile.\n";
		std::cerr << message;
		glDeleteShader(id);
		throw std::runtime_error("Shader failed to compile.");
	}

	return id;
}

// Compile the shaders, link them, provide the identifier
// of the resulting program
static unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	unsigned int program{ glCreateProgram() };
	unsigned int vs{ compile_shader(GL_VERTEX_SHADER, vertex_shader) };
	unsigned int fs{ compile_shader(GL_FRAGMENT_SHADER, fragment_shader) };

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	int result{ };
	char message[512]{};
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		glGetProgramInfoLog(program, 512, NULL, message);
		std::cerr << "Shader program linking failed.\n";
		std::cerr << message << '\n';
		throw std::runtime_error("Shader linking failed.");
	}

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
	if (result == GL_FALSE)
	{
		glGetProgramInfoLog(program, 512, NULL, message);
		std::cerr << "Shader program validation failed.\n";
		std::cerr << message << '\n';
		throw std::runtime_error("Shader validation failed.");
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

static std::string load_shader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string source((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

	return source;
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
	
	float positions[] = {
		200.0, 100.0, 0.0, 0.0,
		600.0, 100.0, 1.0, 0.0,
		600.0, 500.0, 1.0, 1.0,
		200.0, 500.0, 0.0, 1.0
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray va{};
	va.bind();
	VertexBuffer vBuffer{positions, 4 * 4 * sizeof(float) };

	VertexBufferLayout vbLayout{0};
	vbLayout.push<float>(2);
	vbLayout.push<float>(2);
	va.add_buffer(vBuffer, vbLayout);

	IndexBuffer iBuffer{ indices, 2 * 3 };


	Shader shader("texture_v.shader", "texture_f.shader");
	shader.bind();

	//std::array<float, 4> color{ 1.0f, 0.2f, 0.2f, 1.0f };
	//shader.set_uniform_4f("u_color", color);

	Texture texture{"cpp_logo_200.png"};
	texture.bind();
	shader.set_uniform_1i("u_texture", 0);

	// Fix the image aspect ratio via linear transformation (orthonormal projection)
	glm::mat4 projection = glm::ortho(0.0, static_cast<double>(WIDTH),
		0.0, static_cast<double>(HEIGHT));
	shader.set_uniform_mat4("u_mvp", projection);

	// Unbind everything
	va.unbind();
	shader.unbind();
	iBuffer.unbind();
	texture.unbind();

	// Start the main loop
	while (!glfwWindowShouldClose(window))
	{
		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		shader.bind();

		va.bind();
		iBuffer.bind();
		texture.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		va.unbind();
		shader.unbind();

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
