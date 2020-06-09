// main gaming loop implementation

#include "RenderedObject.h"

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

	float positions[] = {
		-0.5, -0.5,
		 0.5, -0.5,
		 0.5,  0.5,
		-0.5, 0.5
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int varray{};
	glGenVertexArrays(1, &varray);
	glBindVertexArray(varray);

	unsigned int buffer{};
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(float), positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	unsigned int ind_buffer{};
	glGenBuffers(1, &ind_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	std::string vertex_shader{ load_shader("basic_v.shader") };
	std::string fragment_shader{ load_shader("basic_f.shader") };

	unsigned int shader{};
	try
	{
		shader = create_shader(vertex_shader, fragment_shader);
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what();
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glUseProgram(shader);
	int location{ glGetUniformLocation(shader, "u_color") };
	assert(location != -1 && "Uniform not found in shader.");
	glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f);

	// Start the main loop
	while (!glfwWindowShouldClose(window))
	{
		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(varray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

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
