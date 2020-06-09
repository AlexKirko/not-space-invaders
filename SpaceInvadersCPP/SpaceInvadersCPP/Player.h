#pragma once

#include "PlayerBullet.h"
#include "RenderedObject.h"
#include "Hitbox.h"

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

	Hitbox m_hitbox;
public:
	Player(const std::array<float, 2>& bottomleft,
		int width, int height,
		bool movable, bool use_textures,
		float angle,
		const std::array<float, 2>& velocity,
		const std::array<float, 4>& color,
		std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures,
		int lives,
		float speed,
		float hitbox_width, float hitbox_height) :
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
		m_pending_bullets{},
		m_hitbox{ hitbox_width, hitbox_height }
	{
		m_hitbox.move_to_r_object(*this);
	}

	void lose_life() { --m_lives; }
	void gain_life() { ++m_lives; }

	int get_lives() { return m_lives; }

	void movement(const MoveTypes& move_type, float speed);

	void tick_time(float time_elapsed);

	void push_bullets(float new_cooldown, float speed);
	// Call this through while loop to make sure all bullets are shot
	std::unique_ptr<PlayerBullet> try_shoot();

	const Hitbox& get_hitbox() const { return m_hitbox; }

	// Consider making virtual if necessity rises
	void move_to(const std::array<float, 2>& bottomleft);
	void gradient_move(float time_elapsed);
};
