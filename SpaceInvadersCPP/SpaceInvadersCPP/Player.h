#pragma once

#include "PlayerBullet.h"
#include "RenderedObject.h"

#include <array>
#include <memory>
#include <vector>
#include <utility>

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
	float m_shoot_cooldown;
	std::vector<
		std::pair<std::unique_ptr<PlayerBullet>, float>
	> m_pending_bullets;
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
			textures },
		m_lives{ lives },
		m_speed{ speed },
		m_shoot_cooldown{ 1.5f },
		m_pending_bullets{}
	{}

	void lose_life() { --m_lives; }
	void gain_life() { ++m_lives; }

	int get_lives() { return m_lives; }

	void movement(const MoveTypes& move_type, float speed);

	void tick_time(float time_elapsed);

	void push_bullets(float new_cooldown, float speed);
	// Call this through while loop to make sure all bullets are shot
	std::unique_ptr<PlayerBullet> try_shoot();
};
