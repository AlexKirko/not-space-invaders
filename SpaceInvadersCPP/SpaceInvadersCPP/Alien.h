#pragma once

#include "AlienBullet.h"
#include "RenderedObject.h"

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
public:
	Alien(const std::array<float, 2>& bottomleft,
		int width, int height,
		bool movable, bool use_textures,
		float angle,
		const std::array<float, 2>& velocity,
		const std::array<float, 4>& color,
		std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures) :
			RenderedObject{ bottomleft,
				width, height,
				movable, use_textures,
				angle,
				velocity,
				color,
				textures },
			m_hp{ 0 }
	{}

	std::unique_ptr<AlienBullet> maybe_shoot(float time_elapsed,
		float window_width,
		float bull_per_second=0.04, float speed=80.0
		);

	void decrease_hp(int damage) { m_hp -= damage; }
	const int get_hp() const { return m_hp; }
};
