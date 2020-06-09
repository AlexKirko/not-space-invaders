#pragma once

#include "Texture.h"
#include "Renderer.h"

#include "Alien.h"
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
	Renderer m_renderer;
	std::vector<std::unique_ptr<Alien>> m_aliens;
	std::shared_ptr<std::vector<std::unique_ptr<Texture>>> m_alien_textures;

	void move_aliens(float time_elapsed);
	void render_aliens();
public:
	Battlefield(float window_width, float window_height);

	void spawn_alien(std::array<float, 2> bottom_left);
	void spawn_alien_row(int min_aliens, float padding);

	void move_objects(float time_elapsed);

	void render_objects();
};