#include "Battlefield.h"

Battlefield::Battlefield(float window_width, float window_height) :
	// Pixels per second
	m_alien_speed{ 30.0f },
	m_player_speed{ 100.0f },
	m_window_width{window_width}, m_window_height{window_height},
	m_alien_textures{ std::make_shared<std::vector<std::unique_ptr<Texture>>>() },
	m_player_textures{ std::make_shared<std::vector<std::unique_ptr<Texture>>>() },
	m_aliens{}, m_alien_bullets{},
	m_player{}, m_player_bullets{},
	m_renderer{ std::make_shared<Renderer>() },
	m_bottom_padding{ 50.0f },
	m_sides_padding{ 100.0f }
{
	// Load test texture.
	auto al_texture1 = std::make_unique<Texture>("res/textures/cpp_logo_200.png");
	m_alien_textures->push_back(std::move(al_texture1));

	// Load player texture
	auto pl_texture1 = std::make_unique<Texture>("res/textures/player.png");
	m_player_textures->push_back(std::move(pl_texture1));
}

void Battlefield::create_player()
{
	std::array<int, 2> player_size{ 60, 60 };
	auto player_ptr = std::make_unique<Player>(
		std::array<float, 2> {
			// Player spawn X coordinate
			m_window_width / 2.0f - static_cast<float>(player_size[0]) / 2.0f,
			// Player spawn Y coordinate
			m_bottom_padding + static_cast<float>(player_size[1])
		},
		player_size[0], player_size[1],
		true, true,
		0.0,
		std::array<float, 2>{0.0f, 0.0f},
		std::array<float, 4>{1.0f, 1.0f, 1.0f, 1.0f},
		m_player_textures,
		3,
		m_player_speed,
		50.0f, 50.0f
		);
	player_ptr->set_current_texture(0);
	player_ptr->register_renderer(m_renderer);
	m_player = std::move(player_ptr);
}

void Battlefield::player_shoots()
{
	while (auto bullet = std::move(m_player->try_shoot()))
	{
		bullet->register_renderer(m_renderer);
		m_player_bullets.push_back(std::move(bullet));
	}
}

void Battlefield::check_hits()
{
	check_alien_hits();
	check_player_hits();
}

void Battlefield::check_player_hits()
{
	// For every player bullet,
	// check if it has impacted any of the aliens
	for (auto& pl_bullet : m_player_bullets)
	{
		// We clean up before rendering
		// so we need to check for an empty pointer here
		for (auto& alien : m_aliens)
		{
			if (pl_bullet != nullptr && alien != nullptr)
			{
				bool hit{
					alien->get_hitbox().check_hit(
						std::array<float, 2> {
							pl_bullet->get_bottomleft()[0] + pl_bullet->get_width() / 2.0f,
							pl_bullet->get_bottomleft()[1] + pl_bullet->get_height() / 2.0f
						}
					)
				};
				if (hit)
				{
					// Remove the bullet and the alien
					pl_bullet.reset();
					alien.reset();
					// TODO : add to score
				}
			}
		}
	}
}

void Battlefield::check_alien_hits()
{
	// For every alien bullet
	// check if it impacted the player
	for (auto& al_bullet : m_alien_bullets)
	{
		if (al_bullet != nullptr)
		{
			bool hit{
				m_player->get_hitbox().check_hit(
					std::array<float, 2> {
						al_bullet->get_bottomleft()[0] + al_bullet->get_width() / 2.0f,
						al_bullet->get_bottomleft()[1] + al_bullet->get_height() / 2.0f
					}
				)
			};
			if (hit)
			{
				// Remove the bullet and decrease health
				al_bullet.reset();
				m_player->lose_life();
			}
		}
	}

	// Check if any of the aliens collided with the player
	for (auto& alien : m_aliens)
	{
		if (alien != nullptr)
		{
			bool hit{
				m_player->get_hitbox().check_hit(alien->get_hitbox())
			};
			if (hit)
			{
				// Remove the bullet and decrease health
				alien.reset();
				m_player->lose_life();
			}
		}
	}
}

void Battlefield::spawn_alien(std::array<float, 2> bottom_left)
{
	auto alien_ptr = std::make_unique<Alien>(
		bottom_left,
		50, 50,
		true, true,
		0.0,
		std::array<float, 2>{0.0f, -m_alien_speed},
		std::array<float, 4>{1.0, 1.0, 1.0, 1.0},
		m_alien_textures,
		// Alien hitbox size
		40.0, 40.0
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
	move_player(time_elapsed);
	move_player_bullets(time_elapsed);
}

void Battlefield::render_objects()
{
	// Render everything on the battlefield
	// Be mindful of priority (front objects rendering last)
	/*
	render_player();
	render_aliens();
	render_alien_bullets();
	render_player_bullets();
	*/

	render(m_player);
	render(m_aliens);
	render(m_alien_bullets);
	render(m_player_bullets);

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
	bool cleanup{ false };
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
}

void Battlefield::move_player_bullets(float time_elapsed)
{
	bool cleanup{ false };
	for (auto& player_bullet : m_player_bullets)
	{
		player_bullet->gradient_move(time_elapsed);
		if (player_bullet->get_bottomleft()[1] > m_window_height)
		{
			// Destroy off-screen bullet
			// This leaves empty pointers in m_player_bullets
			// but avoids constant resizing
			player_bullet.reset();
			cleanup = true;
		}
	}
}

void Battlefield::move_player(float time_elapsed)
{
	m_player->gradient_move(time_elapsed);
	// Check that we didn't move the player beyond the battlefield
	// If so, move back and set speed to 0
	bool out_of_field{ false };
	if (m_player->get_bottomleft()[0] < m_sides_padding)
	{
		out_of_field = true;
		m_player->move_to(std::array<float, 2>{
			m_sides_padding,
			m_player->get_bottomleft()[1]
		});
	}
	else if (m_player->get_bottomleft()[0] + m_player->get_width() > 
		m_window_width - m_sides_padding)
	{
		out_of_field = true;
		m_player->move_to(std::array<float, 2>{
			m_window_width - m_sides_padding - m_player->get_width(),
			m_player->get_bottomleft()[1]
		});
	}

	if (out_of_field)
	{
		m_player->movement(MoveTypes::STILL, m_player_speed);
	}
}


template<typename rObjType>
void Battlefield::render(std::unique_ptr<rObjType>& r_object)
{
	m_renderer->init_render(*r_object, m_window_width, m_window_height);
	m_renderer->render(*r_object, m_window_width, m_window_height);
}


template<typename rObjType>
void Battlefield::render(std::vector<std::unique_ptr<rObjType>>& r_objects)
{
	// Clean up empty pointers before rendering
	if (!r_objects.empty())
	{
		r_objects.erase(std::remove_if(
			r_objects.begin(),
			r_objects.end(),
			[](std::unique_ptr<rObjType>& r_object)
			{
				return r_object == nullptr;
			}
		),
		r_objects.end());
	}
	
	for (auto& r_object : r_objects)
	{
		render(r_object);
	}
}
