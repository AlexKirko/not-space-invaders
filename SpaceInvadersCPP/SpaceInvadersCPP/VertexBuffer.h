#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class VertexBuffer
{
private:
	unsigned int m_uid;

public:
	// Basically, we want to be able to put anything
	// into the VertexBuffer.
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};