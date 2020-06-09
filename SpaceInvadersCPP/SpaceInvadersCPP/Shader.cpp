#include "Shader.h"


const std::string Shader::load_shader(const std::string& filepath) const
{
	std::ifstream stream(filepath);
	// For some reason, this doesn't work without the extra parentheses
	std::string source((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

	return source;
}

const unsigned int Shader::compile_shader(unsigned int type, const std::string& source) const
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

const int Shader::get_uniform_location(const std::string& name) const
{
	int location{ glGetUniformLocation(m_uid, name.c_str()) };
	if (location == -1)
	{
		throw std::runtime_error(name + " uniform not found in shader.");
	}
	return location;
}

Shader::Shader()
{
	m_uid = glCreateProgram();
}

Shader::Shader(const std::string& vertex_filepath, const std::string& fragment_filepath)
{
	m_uid = glCreateProgram();
	
	reset_shaders(vertex_filepath, fragment_filepath);
}

Shader::~Shader()
{
	glDeleteProgram(m_uid);
}

void Shader::reset_shaders(const std::string& vertex_filepath, const std::string& fragment_filepath)
{
	std::string vertex_shader{ load_shader(vertex_filepath) };
	std::string fragment_shader{ load_shader(fragment_filepath) };
	unsigned int vs{ compile_shader(GL_VERTEX_SHADER, vertex_shader) };
	unsigned int fs{ compile_shader(GL_FRAGMENT_SHADER, fragment_shader) };

	glAttachShader(m_uid, vs);
	glAttachShader(m_uid, fs);

	glLinkProgram(m_uid);
	int result{ };
	char message[512]{};
	glGetProgramiv(m_uid, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		glGetProgramInfoLog(m_uid, 512, NULL, message);
		std::cerr << "Shader m_uid linking failed.\n";
		std::cerr << message << '\n';
		throw std::runtime_error("Shader linking failed.");
	}

	glValidateProgram(m_uid);
	glGetProgramiv(m_uid, GL_VALIDATE_STATUS, &result);
	if (result == GL_FALSE)
	{
		glGetProgramInfoLog(m_uid, 512, NULL, message);
		std::cerr << "Shader m_uid validation failed.\n";
		std::cerr << message << '\n';
		throw std::runtime_error("Shader validation failed.");
	}

	glDetachShader(m_uid, vs);
	glDetachShader(m_uid, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::bind() const
{
	glUseProgram(m_uid);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::set_uniform_4f(const std::string& name, const std::array<float, 4>& floats) const
{
	int location{ get_uniform_location(name) };
	glUniform4f(location, floats[0], floats[1], floats[2], floats[3]);
}

void Shader::set_uniform_1i(const std::string& name, unsigned int ind) const
{
	int location{ get_uniform_location(name) };
	glUniform1i(location, ind);
}

void Shader::set_uniform_mat4(const std::string& name, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]);
}
