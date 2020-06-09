#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "stb_image.h"

#include <string>

class Texture
{
private:
	unsigned int m_uid;
	// bpp is bits per pixel
	int m_width, m_height, m_bpp;
	// This is where we'll be storing the texture
	unsigned char* m_image_buffer;

public:
	Texture(const std::string& filepath);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	const int get_width() { return m_width; }
	const int get_height() { return m_height; }
};