#pragma once

#include "Texture.h"

#include <array>
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>

// Implements functionality that all rendered objects should have
class RenderedObject
{
private:
	std::array<float, 2> m_bottomleft;
	// Width and height are in pixels
	const int m_width;
	const int m_height;
	const bool m_movable;
	bool m_use_textures;
	float m_angle;
	std::array<float, 2> m_velocity;
	std::array<float, 4> m_color;

	int m_current_texture;
	std::shared_ptr<std::vector<Texture>> m_textures;

public:
	RenderedObject(
		const std::array<float, 2>& bottomleft,
		int width, int height,
		bool movable,
		bool use_textures=false,
		float angle=0.0,
		const std::array<float, 2>& velocity = std::array<float, 2>{0.0, 0.0},
		const std::array<float, 4>& color = std::array<float, 4>{1.0, 1.0, 1.0, 1.0},
		std::shared_ptr<std::vector<Texture>> textures = std::shared_ptr<std::vector<Texture>>{nullptr});

	void move_to(const std::array<float, 2>& bottomleft);
	void set_velocity(const std::array<float, 2>& velocity);

	void set_textures(std::shared_ptr<std::vector<Texture>> textures);

	void set_current_texture(int bitmap_ind);
	const int get_current_texture() const;

	const bool get_use_textures() const { return m_use_textures; }

	void set_angle(float angle) { m_angle = angle;	}
	const float get_angle() const { return m_angle; }

	const std::array<float, 2>& get_bottomleft() const { return m_bottomleft; }
};
