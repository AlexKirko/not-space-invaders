#pragma once

#include "RenderedObject.h"

class Hitbox
{
private:
	// Width and height
	float m_width;
	float m_height;
	// Left, right, bottom, up
	float m_lbound;
	float m_rbound;
	float m_bbound;
	float m_ubound;
public:
	Hitbox(float width, float height) :
		m_width{width}, m_height{height},
		m_lbound{0.0}, m_rbound{0.0}, m_bbound{0.0}, m_ubound{0.0}
	{}

	void move_to_r_object(const RenderedObject& rObj);
	// Check for collision with a RenderedObject without a hitbox
	const bool check_hit(std::array<float, 2> check_coords) const
	{
		return (check_coords[0] > m_lbound && check_coords[0] < m_rbound
			&& check_coords[1] > m_bbound && check_coords[1] < m_ubound);
	}

	// Check for collision with another hitbox
	const bool check_hit(const Hitbox& other) const
	{
		return (
			(m_ubound > other.m_bbound && m_bbound < other.m_ubound) &&
			(m_lbound < other.m_rbound && m_rbound > other.m_lbound)
		);
	}
};
