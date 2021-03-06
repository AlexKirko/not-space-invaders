#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#define GLEW_STATIC
#include <GL/glew.h>

class VertexArray
{
private:
	unsigned int m_uid;

public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;

	void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& vbLayout);
};