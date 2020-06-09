#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <array>
#include <map>
#include <memory>
#include <utility>

// Forward-declare RenderedObject to enable references
class RenderedObject;

/*
Render a RenderedObject on screen.

Aimed to hide the OpenGL implementation from the user
and allow sprite (or rectangle) rendering with one call.
*/
class Renderer
{
private:
	
	struct RenderData
	{
		std::unique_ptr<VertexArray> va;
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;
		std::unique_ptr<Shader> shader;

		RenderData(
		std::unique_ptr<VertexArray>& in_va,
		std::unique_ptr<VertexBuffer>& in_vb,
		std::unique_ptr<IndexBuffer>& in_ib,
		std::unique_ptr<Shader>& in_shader)
	{
		// Move the pointers into the structure
		va = std::move(in_va);
		vb = std::move(in_vb);
		ib = std::move(in_ib);
		shader = std::move(in_shader);
	}
	};
	
	std::map<int, std::unique_ptr<RenderData>> m_cached_rdata;

public:
	Renderer();

	void init_render(const RenderedObject& rObj,
		int s_width, int s_height);
	void render(const RenderedObject& rObj,
		int s_width, int s_height);
	void clear(const std::array<float, 4>& bckgrnd_color);
	void remove_from_cache(int rObj_uid);
};