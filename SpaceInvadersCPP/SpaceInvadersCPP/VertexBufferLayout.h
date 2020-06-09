#pragma once

// GLEW: for loading OpenGL functions newer than OpenGL 1.1
#define GLEW_STATIC
#include <GL/glew.h>

#include <cassert>
#include <stdexcept>
#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int get_size(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return sizeof(GLfloat);
			case GL_UNSIGNED_INT: return sizeof(GLuint);
			case GL_UNSIGNED_BYTE: return sizeof(GLchar);
		}
		throw std::runtime_error("Quering size on invalid VertexBufferElement type.");
		return 0;
	}
};


class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
public:
	VertexBufferLayout(unsigned int stride=0) : m_stride{ 0 }
	{
	}
	~VertexBufferLayout()
	{
	}

	template<typename T>
	void push(unsigned int count)
	{
		assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
		m_stride += count * VertexBufferElement::get_size(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_elements.push_back(VertexBufferElement{ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * VertexBufferElement::get_size(GL_UNSIGNED_INT);
	}

	template<>
	void push<char>(unsigned int count)
	{
		m_elements.push_back(VertexBufferElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * VertexBufferElement::get_size(GL_UNSIGNED_BYTE);
	}

	const std::vector<VertexBufferElement>& get_elements() const { return m_elements; }
	const unsigned int get_stride() const { return m_stride; }
};