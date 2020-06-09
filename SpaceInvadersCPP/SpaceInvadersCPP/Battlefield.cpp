#include "Battlefield.h"

Battlefield::Battlefield(float window_width, float window_height) : 
	m_alien_speed{1.0f},
	m_window_width{window_width}, m_window_height{window_height},
	m_renderer{}, m_alien_textures{}
{
	m_alien_textures = std::make_shared<std::vector<std::unique_ptr<Texture>>>();

	// Load test texture.
	auto texture_ptr = std::make_unique<Texture>("res/textures/cpp_logo_200.png");
	texture_ptr->bind();
	m_alien_textures->push_back(std::move(texture_ptr));
	texture_ptr->unbind();
}

void Battlefield::spawn_alien(std::array<float, 2> bottom_left)
{
	auto alien_ptr = std::make_unique<Alien>(
		bottom_left,
		50, 50,
		true, true,
		0.0,
		std::array<float, 2>{0.0f, m_alien_speed * -30.0f},
		std::array<float, 4>{1.0, 1.0, 1.0, 1.0},
		m_alien_textures
	);
	alien_ptr->set_current_texture(0);
	m_aliens.push_back(std::move(alien_ptr));
}

void Battlefield::spawn_alien_row(int min_aliens, float padding)
{
	// Spawn aliens if there are no aliens or if there are too few
	// and there is space for a new row.
	// We rely on the program checking the first condition first here
	// but this should be so on any reasonable system
	if (m_aliens.empty() ||
		(
			m_aliens.size() < min_aliens &&
			(
				m_aliens.back()->get_bottomleft()[1] +
				m_aliens.back()->get_height() + padding < m_window_height
				)
			)
		)
	{
		float spawn_height{m_window_height};
		for (int i{ 0 }; i < 10; ++i)
		{
			spawn_alien(std::array<float, 2>{ 100.0f + 60.0f * i, spawn_height });
		}
	}
}

void Battlefield::move_objects(float time_elapsed)
{
	// Move all movable objects according to velocity
	move_aliens(time_elapsed);
}

void Battlefield::render_objects()
{
	// Render everything on the battlefield
	// Be mindful of priority (front objects rendering last)

	render_aliens();
}

void Battlefield::move_aliens(float time_elapsed)
{
	for (auto& alien_ptr : m_aliens)
	{
		alien_ptr->gradient_move(time_elapsed);
	}
}

void Battlefield::render_aliens()
{
	for (auto& alien : m_aliens)
	{
		m_renderer.init_render(*alien, m_window_width, m_window_height);
		m_renderer.render(*alien, m_window_width, m_window_height);
	}
}
