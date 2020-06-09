#include "Hitbox.h"

void Hitbox::move_to_r_object(const RenderedObject& rObj)
{
	// Move hitbox boundaries to the new RenderedObject location
	m_lbound = rObj.get_bottomleft()[0] + (rObj.get_width() - m_width) / 2.0f;
	m_rbound = rObj.get_bottomleft()[0] + (rObj.get_width() + m_width) / 2.0f;
	m_bbound = rObj.get_bottomleft()[1] + (rObj.get_height() - m_height) / 2.0f;
	m_ubound = rObj.get_bottomleft()[1] + (rObj.get_height() + m_height) / 2.0f;
}
