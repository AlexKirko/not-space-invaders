#include "Texture.h"

Texture::Texture(const std::string& filepath) :
	m_uid{ 0 }, m_width { 0 }, m_height{ 0 }, m_bpp{ 0 }, m_image_buffer(nullptr)
{
	stbi_set_flip_vertically_on_load(1);
	m_image_buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_bpp, 4);

	glGenTextures(1, &m_uid);
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image_buffer);
	unbind();

	if (m_image_buffer)
	{
		stbi_image_free(m_image_buffer);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_uid);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_uid);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
