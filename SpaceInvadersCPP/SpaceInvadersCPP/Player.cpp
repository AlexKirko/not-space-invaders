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
