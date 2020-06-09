#include "Battlefield.h"

Battlefield::Battlefield(float window_width, float window_height) : 
	m_alien_speed{1.0f},
	m_window_width{window_width}, m_window_height{window_height},
	m_alien_textures{}, m_aliens{}, m_alien_bullets{}
{
	m_renderer = std::make_shared<Renderer>();
	m_alien_textures = std::make_shared<std::vector<std::unique_ptr<Texture>>>();

	// Load test texture.
	auto texture_ptr = std::make_unique<Texture>("res/textures/cpp_logo_200.png");
	m_alien_textures->push_back(std::move(texture_ptr));
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
	alien_ptr->register_renderer(m_renderer);
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

void Battlefield::aliens_shoot(float time_elapsed)
{
	for (auto& alien_ptr : m_aliens)
	{
		std::unique_ptr<AlienBullet> alien_bull_ptr{
			alien_ptr->maybe_shoot(time_elapsed, m_window_width)};

		if (alien_bull_ptr)
		{
			alien_bull_ptr->register_renderer(m_renderer);
			m_alien_bullets.push_back(std::move(alien_bull_ptr));
		}
	}
}

void Battlefield::move_objects(float time_elapsed)
{
	// Move all movable objects according to velocity
	move_aliens(time_elapsed);
	move_alien_bullets(time_elapsed);
}

void Battlefield::render_objects()
{
	// Render everything on the battlefield
	// Be mindful of priority (front objects rendering last)

	render_aliens();
	render_alien_bullets();
}

void Battlefield::move_aliens(float time_elapsed)
{
	for (auto& alien : m_aliens)
	{
		alien->gradient_move(time_elapsed);
	}
}

void Battlefield::move_alien_bullets(float time_elapsed)
{
	bool cleanup = false;
	for (auto& alien_bullet : m_alien_bullets)
	{
		alien_bullet->gradient_move(time_elapsed);
		if (alien_bullet->get_bottomleft()[1] + alien_bullet->get_height() < 0)
		{
			// Destroy off-screen bullet
			// This leaves empty pointers in m_alien_bullets
			// but avoids constant resizing
			alien_bullet.reset();
			cleanup = true;
		}
	}
	if (cleanup)
	{
		m_alien_bullets.erase(std::remove_if(
			m_alien_bullets.begin(), 
			m_alien_bullets.end(),
			[](std::unique_ptr<AlienBullet>& alien_bullet)
			{
				return alien_bullet == nullptr;
			}
			));
	}
}

void Battlefield::render_aliens()
{
	for (auto& alien : m_aliens)
	{
		m_renderer->init_render(*alien, m_window_width, m_window_height);
		m_renderer->render(*alien, m_window_width, m_window_height);
	}
}

void Battlefield::render_alien_bullets()
{
	for (auto& alien_bullet : m_alien_bullets)
	{
		m_renderer->init_render(*alien_bullet, m_window_width, m_window_height);
		m_renderer->render(*alien_bullet, m_window_width, m_window_height);
	}
}

