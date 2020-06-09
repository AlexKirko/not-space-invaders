#pragma once

// GLEW: for loading OpenGL functions newer than OpenGL 1.1
#define GLEW_STATIC
#include <GL/glew.h>

class IndexBuffer
{
private:
	unsigned int m_uid;
	unsigned int m_count;

public:
	// Basically, we want to be able to put anything
	// into the IndexBuffer.
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	unsigned int get_count() { return m_count; }
};