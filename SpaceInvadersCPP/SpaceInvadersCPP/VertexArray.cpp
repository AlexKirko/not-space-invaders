#include "VertexArray.h"

#include <iostream>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_uid);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_uid);
}

void VertexArray::bind() const
{
	glBindVertexArray(m_uid);
}

void VertexArray::unbind() const
{
	glBindVertexArray(m_uid);
}

void VertexArray::add_buffer(const VertexBuffer& vb, const VertexBufferLayout& vbLayout)
{
	vb.bind();
	const auto& elements = vbLayout.get_elements();
	int offset{ 0 };
	for (const auto& element : elements)
	{
		glVertexAttribPointer(0, element.count, element.type,
			element.normalized, vbLayout.get_stride(), (const void*)offset);
		glEnableVertexAttribArray(0);
		offset += VertexBufferElement::get_size(element.type);
	}
}
