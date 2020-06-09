#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <array>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <string>

class Shader
{
private:
	unsigned int m_uid;

	const std::string load_shader(const std::string& filepath) const;
	const unsigned int compile_shader(unsigned int type, const std::string& source) const;
	const int get_uniform_location(const std::string& name) const;
public:
	Shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void set_uniform_4f(const std::string& name, const std::array<float, 4>& floats) const;
	void set_uniform_1i(const std::string& name, unsigned int ind) const;
};
