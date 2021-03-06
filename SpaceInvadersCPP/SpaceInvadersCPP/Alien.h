#pragma once

#include "AlienBullet.h"
#include "RenderedObject.h"
#include "Hitbox.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/vector_angle.hpp>

#include <cmath>
#include <array>
#include <ctime>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <random>

class Alien : public RenderedObject
{
private:
	static std::mt19937 random_seeder;
	static std::uniform_int_distribution<int> rng;

	int m_hp;
	int m_score;

	Hitbox m_hitbox;
public:
	Alien(const std::array<float, 2>& bottomleft,
		int width, int height,
		bool movable, bool use_textures,
		float angle,
		const std::array<float, 2>& velocity,
		const std::array<float, 4>& color,
		std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures,
		float hitbox_width, float hitbox_height) :
			RenderedObject{ bottomleft,
				width, height,
				movable, use_textures,
				angle,
				velocity,
				color,
				textures },
			m_hp{ 1 }, m_score{ 100 },
		m_hitbox{ hitbox_width, hitbox_height}
	{ 
		m_hitbox.move_to_r_object(*this);
	}

	std::unique_ptr<AlienBullet> maybe_shoot(float time_elapsed,
		float window_width,
		float bull_per_second=0.04, float speed=80.0
		);

	void decrease_hp(int damage) { m_hp -= damage; }
	const int get_hp() const { return m_hp; }
	const int get_score() const { return m_score; }
	const Hitbox& get_hitbox() const { return m_hitbox; }

	// Consider making virtual if necessity rises
	void move_to(const std::array<float, 2>& bottomleft);
	void gradient_move(float time_elapsed);
};
