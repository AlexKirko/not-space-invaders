#pragma once

#include "AlienBullet.h"
#include "RenderedObject.h"

#include <array>

enum class MoveTypes
{
	LEFT,
	RIGHT,
	STILL,
};

class Player : public RenderedObject
{
private:
	int m_lives;
	float m_speed;
	float m_speed_factor;
public:
	Player(const std::array<float, 2>& bottomleft,
		int width, int height,
		bool movable, bool use_textures,
		float angle,
		const std::array<float, 2>& velocity,
		const std::array<float, 4>& color,
		std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures,
		int lives,
		float speed) :
		RenderedObject{ bottomleft,
			width, height,
			movable, use_textures,
			angle,
			velocity,
			color,
			textures }, m_lives{ lives },
			m_speed{ speed }
	{}

	void lose_life() { --m_lives; }
	void gain_life() { ++m_lives; }

	int get_lives() { return m_lives; }

	void movement(const MoveTypes& move_type, float speed);
};
