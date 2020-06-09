#include "Player.h"

void Player::movement(const MoveTypes& move_type, float speed)
{
	std::array<float, 2> velocity{ 0.0, 0.0 };
	switch (move_type)
	{
	case MoveTypes::LEFT:
		velocity[0] = -m_speed;
		break;
	case MoveTypes::RIGHT:
		velocity[0] = m_speed;
		break;
	default:
		break;
	}
	set_velocity(velocity);
}

void Player::tick_time(float time_elapsed)
{
	m_shoot_cooldown -= time_elapsed;
	for (auto& bullet : m_pending_bullets)
	{
		bullet.second -= time_elapsed;
	}
}

void Player::push_bullets(float new_cooldown, float speed)
{
	if (m_shoot_cooldown <= 0.0f)
	{
		// Add two bullets to shoot in quick succession
		float delay = 0.0f;
		for (int i{ 0 }; i < 2; ++i)
		{
			// Some extra resizing here, but we can afford it
			m_pending_bullets.insert(m_pending_bullets.begin(),
				std::pair<std::unique_ptr<PlayerBullet>, float> {
					std::make_unique<PlayerBullet>(
						// We set origin with move_to when we actually shoot
						std::array<float, 2>{ 0.0f, 0.0f },
						5, 20,
						true, false,
						0.0f,
						std::array<float, 2>{ 0.0f, speed },
						std::array<float, 4>{1.0, 1.0, 1.0, 1.0}
					),
					delay
				}
			);

			delay += 0.5f;
		}

		m_shoot_cooldown = new_cooldown;
	}
}

std::unique_ptr<PlayerBullet> Player::try_shoot()
{
	if (!m_pending_bullets.empty())
	{
		if (m_pending_bullets.back().second <= 0.0f)
		{
			auto bullet = std::move(m_pending_bullets.back().first);
			m_pending_bullets.pop_back();

			// Set origin
			bullet->move_to(std::array<float, 2>{
				// Set starting X coordinate
				get_bottomleft()[0] + get_width() / 2.0f - bullet->get_width() / 2.0f,
					// Set starting Y coordinate
					get_bottomleft()[1] + get_height()
			});

			return bullet;
		}
	}
	return nullptr;
}
