#include "Renderer.h"

Renderer::Renderer() : m_cached_rdata{}
{
}

void Renderer::init_render(const RenderedObject& rObj,
	int s_width, int s_height)
{
	std::array<float, 2> b_left = rObj.get_bottomleft();
	float width = rObj.get_width();
	float height = rObj.get_height();


	float texture_positions[] = {
		b_left[0],         b_left[1],          0.0, 0.0,
		b_left[0] + width, b_left[1],          1.0, 0.0,
		b_left[0] + width, b_left[1] + height, 1.0, 1.0,
		b_left[0],         b_left[1] + height, 0.0, 1.0
	};

	float basic_positions[] = {
		b_left[0],         b_left[1],
		b_left[0] + width, b_left[1],
		b_left[0] + width, b_left[1] + height,
		b_left[0],         b_left[1] + height,
	};

	// This index buffer draws a rectangle
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	auto va_ptr = std::make_unique<VertexArray>();
	va_ptr->bind();
	// Need to presere the VertexBuffer too
	auto vb_ptr = std::make_unique<VertexBuffer>(
		rObj.get_use_textures() ? texture_positions : basic_positions, 
		4 * 4 * sizeof(float));

	VertexBufferLayout vbLayout{ 0 };
	vbLayout.push<float>(2);
	if (rObj.get_use_textures())
		vbLayout.push<float>(2);
	va_ptr->add_buffer(*vb_ptr, vbLayout);

	auto ib_ptr = std::make_unique<IndexBuffer>(indices, 2 * 3);

	// Choose shader (sprite or monochrome)
	auto shader_ptr = std::make_unique<Shader>();
	if (rObj.get_use_textures())
	{
		shader_ptr->reset_shaders("texture_v.shader", "texture_f.shader");
		shader_ptr->bind();
	}
	else
	{
		shader_ptr->reset_shaders("basic_v.shader", "basic_f.shader");
		shader_ptr->bind();
	}

	//std::array<float, 4> color{ 1.0f, 0.2f, 0.2f, 1.0f };
	//shader.set_uniform_4f("u_color", color);
	if (rObj.get_use_textures())
	{
		// Since this is a basic sprite engine, we don't use
		// layered textures, so the slot is always 0.
		shader_ptr->set_uniform_1i("u_texture", 0);
	}
	else
	{
		shader_ptr->set_uniform_4f("u_color", rObj.get_color());
	}

	// Fix the image aspect ratio via linear transformation (orthonormal projection)
	glm::mat4 projection = glm::ortho(0.0, static_cast<double>(s_width),
		0.0, static_cast<double>(s_height));
	shader_ptr->set_uniform_mat4("u_mvp", projection);

	// Unbind everything
	va_ptr->unbind();
	shader_ptr->unbind();
	ib_ptr->unbind();

	// Cache render data in the object
	auto rData_ptr = std::make_unique<RenderData>(va_ptr, vb_ptr, ib_ptr, shader_ptr);
	std::pair<const RenderedObject*, std::unique_ptr<RenderData>> to_insert =
		std::make_pair( &rObj, std::move(rData_ptr) );
	m_cached_rdata.insert(std::move(to_insert));
}

// Always init_render before render just in case the program
// reuses a memory address for another RenderedObject
void Renderer::render(const RenderedObject* const r_obj_ptr,
	int s_width, int s_height)
{
	auto pos = m_cached_rdata.find(r_obj_ptr);
	// Silently init if we didn't init before
	// Warning: not completely safe. Relies on object pointers
	// being unique.
	if (pos == m_cached_rdata.end())
	{
		init_render(*r_obj_ptr, s_width, s_height);
		// The lookup can be refactored away.
		// But we should always init before render anyway.
		pos = m_cached_rdata.find(r_obj_ptr);
	}

	(*pos->second).shader->bind();
	(*pos->second).va->bind();
	(*pos->second).ib->bind();
	if (r_obj_ptr->get_use_textures())
		r_obj_ptr->get_current_texture().bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	(*pos->second).shader->unbind();
	(*pos->second).va->unbind();
	(*pos->second).ib->unbind();
	if (r_obj_ptr->get_use_textures())
		r_obj_ptr->get_current_texture().unbind();
}

void Renderer::clear(const std::array<float, 4>& bckgrnd_color)
{
	glClearColor(bckgrnd_color[0], bckgrnd_color[1],
		bckgrnd_color[2], bckgrnd_color[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}
