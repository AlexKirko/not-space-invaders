#include "Alien.h"

// Initialize random number generator
std::mt19937 Alien::random_seeder{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
std::uniform_int_distribution<int> Alien::rng{ 0, RAND_MAX };

std::unique_ptr<AlienBullet> Alien::maybe_shoot(float time_elapsed,
	float window_width,
	float bull_per_second, float speed)
{	
	if (static_cast<float>(Alien::rng(Alien::random_seeder)) / 
		static_cast<float>(RAND_MAX) < bull_per_second * time_elapsed)
	{
		// Shoot
		// An alien shoots at the bottom of the screen randomly
		float rand_val{ static_cast<float>(Alien::rng(Alien::random_seeder)) / 
			static_cast<float>(RAND_MAX) };
		
		std::array<float, 2> origin{ get_bottomleft()[0] + get_width() / 2,
			get_bottomleft()[1] };

		// Trouble shoot the formulas : firing arc too narrow
		float x_max_l_velocity{ -std::sqrt(static_cast<float>(std::pow(speed, 2.0)) *
			origin[0] / (origin[0] + origin[1])) };
		float x_max_r_velocity{ std::sqrt(static_cast<float>(std::pow(speed, 2.0)) *
			(window_width - origin[0]) /
			(window_width - origin[0] + origin[1])) };

		float x_velocity{ x_max_l_velocity + 
			(x_max_r_velocity - x_max_l_velocity) * rand_val };

		std::array<float, 2> velocity{
			x_velocity,
			-std::sqrt(std::pow(speed, 2.0) - std::pow(x_velocity, 2.0))
		};

		// Get angle from velocity
		float angle = glm::degrees(glm::angle(
			glm::normalize(glm::vec2{ 0.0, -1.0 }),
			glm::normalize(glm::vec2{ velocity[0], velocity[1] })));

		angle = velocity[0] > 0 ? -angle : angle;

		std::unique_ptr<AlienBullet> alien_bull_ptr = std::make_unique<AlienBullet>(
			origin,
			5, 20,
			true, false,
			angle,
			velocity,
			std::array<float, 4>{1.0, 1.0, 1.0, 1.0}
			);

		return alien_bull_ptr;
	}
	else
	{
		return nullptr;
	}
}
