#include "RenderedObject.h"

int RenderedObject::rObj_count{ 0 };

RenderedObject::RenderedObject(
	const std::array<float, 2>& bottomleft,
	int width, int height,
	bool movable, bool use_textures,
	float angle,
	const std::array<float, 2>& velocity,
	const std::array<float, 4>& color,
	std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures) :
	m_bottomleft{bottomleft}, m_angle{angle},
	m_width{width}, m_height{height},
	m_movable{movable}, m_use_textures{use_textures},
	m_color{color}, m_current_texture{-1}, m_velocity{0.0, 0.0},
	m_textures{nullptr}
{
	// Check for conflicts
	if (!movable && velocity != std::array<float, 2>{0.0, 0.0})
	{
		throw std::runtime_error("Attempted to set non-zero"
		" velocity on an unmovable object.");
	}
	if (!use_textures &&
		textures != std::shared_ptr<std::vector<std::unique_ptr<Texture>>>{})
	{
		throw std::runtime_error("Attempted to set textures"
			" velocity on an object that doesn't use textures.");
	}
	m_velocity = velocity;
	m_textures = textures;

	// Record unique id
	m_uid = rObj_count;
	++rObj_count;
}

RenderedObject::RenderedObject(const RenderedObject& other) :
	m_bottomleft{other.m_bottomleft}, m_angle{other.m_angle},
	m_width{other.m_width}, m_height{other.m_height},
	m_movable{other.m_movable}, m_use_textures{other.m_use_textures},
	m_color{other.m_color}, m_current_texture{other.m_current_texture},
	m_velocity{other.m_velocity}, m_textures{other.m_textures}
{
	// Record unique id
	m_uid = rObj_count;
	++rObj_count;
}

void RenderedObject::move_to(const std::array<float, 2>& bottomleft)
{
	// Move the shape to a new location
	// Check if the object is movable
	if (!m_movable)
	{
		throw std::runtime_error("Tried to move unmovable rendered object.");
	}

	// Move the object
	m_bottomleft = bottomleft;
}

void RenderedObject::set_velocity(const std::array<float, 2>& velocity)
{
	// Set new velocity
	// Check if the object is movable
	if (!m_movable)
	{
		throw std::runtime_error("Tried to assign velocity to unmovable rendered object.");
	}

	m_velocity = velocity;
}

void RenderedObject::set_textures(std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures)
{
	// Textures are created by switching to a new game state
	// objects share textures to avoid duplication
	m_use_textures = true;
	auto m_textures = textures;
}

void RenderedObject::set_current_texture(int current_texture)
{
	if (!m_use_textures)
	{
		throw std::runtime_error("Can't set current texture on"
			" a Rendered object that doesn't use textures.");
	}
	m_current_texture = current_texture;
}

const Texture& RenderedObject::get_current_texture() const
{
	if (!m_use_textures)
	{
		throw std::runtime_error("Current texture not set"
			" or not in use.");
	}
	return *(*m_textures)[m_current_texture];
}

void RenderedObject::gradient_move(float time_elapsed)
{
	move_to(std::array<float, 2> {
		m_bottomleft[0] + m_velocity[0] * time_elapsed,
			m_bottomleft[1] + m_velocity[1] * time_elapsed
	});
}
