#pragma once

#include "Texture.h"
#include "Renderer.h"

#include "Alien.h"
#include "AlienBullet.h"
#include "RenderedObject.h"

#include <memory>
#include <vector>

// Manage all the game mechanics
class Battlefield
{
private:
	float m_window_width;
	float m_window_height;
	float m_alien_speed;
	std::shared_ptr<Renderer> m_renderer;
	std::vector<std::unique_ptr<Alien>> m_aliens;
	std::vector<std::unique_ptr<AlienBullet>> m_alien_bullets;
	std::shared_ptr<std::vector<std::unique_ptr<Texture>>> m_alien_textures;

	void move_aliens(float time_elapsed);
	void move_alien_bullets(float time_elapsed);
	void render_aliens();
	void render_alien_bullets();
public:
	Battlefield(float window_width, float window_height);

	void spawn_alien(std::array<float, 2> bottom_left);
	void spawn_alien_row(int min_aliens, float padding);

	void aliens_shoot(float time_elapsed);

	void move_objects(float time_elapsed);
	void render_objects();
};