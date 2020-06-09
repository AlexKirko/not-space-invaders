#include "RenderedObject.h"

void RenderedObject::move_to(const std::array<double, 2>& center)
{
	// Move the shape to a new center
	// Check if the object is movable
	if (!m_movable)
	{
		throw std::runtime_error("Tried to move unmovable rendered object.");
	}

	// Move the object center
	m_center = center;
}

void RenderedObject::set_velocity(const std::array<double, 2>& velocity)
{
	// Set new velocity
	// Check if the object is movable
	if (!m_movable)
	{
		throw std::runtime_error("Tried to assign velocity to unmovable rendered object.");
	}

	m_velocity = velocity;
}