#pragma once

#include "RenderedObject.h"

class PlayerBullet : public RenderedObject
{
private:
	int m_damage;
public:
	PlayerBullet(const std::array<float, 2>& bottomleft,
		int width, int height,
		bool movable, bool use_textures,
		float angle,
		const std::array<float, 2>& velocity,
		const std::array<float, 4>& color,
		std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures = nullptr,
		int damage = 1) :
		RenderedObject{ bottomleft,
			width, height,
			movable, use_textures,
			angle,
			velocity,
			color,
			textures },
		m_damage{ damage }
	{}

	const int get_damage() const { return m_damage; };
};