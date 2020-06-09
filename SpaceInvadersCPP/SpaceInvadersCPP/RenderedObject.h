#pragma once

#include "Texture.h"

#include <array>
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>

// Forward-declarer Renderer to enable pointers
class Renderer;

// Implements functionality that all rendered objects should have
class RenderedObject
{
private:
	int m_uid;
	std::array<float, 2> m_bottomleft;
	// Width and height are in pixels
	const int m_width;
	const int m_height;
	const bool m_movable;
	bool m_use_textures;
	float m_angle;
	std::array<float, 2> m_velocity;
	std::array<float, 4> m_color;
	// left, down, up, right
	std::array<float, 4> m_tex_smpl_coords;

	int m_current_texture;
	std::shared_ptr<std::vector<std::unique_ptr<Texture>>> m_textures;
	std::shared_ptr<Renderer> m_renderer;

	static int rObj_count;
public:
	RenderedObject(
		const std::array<float, 2>& bottomleft,
		int width, int height,
		bool movable,
		bool use_textures=false,
		float angle=0.0,
		const std::array<float, 2>& velocity = std::array<float, 2>{0.0, 0.0},
		const std::array<float, 4>& color = std::array<float, 4>{1.0, 1.0, 1.0, 1.0},
		std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures =
		std::shared_ptr<std::vector<std::unique_ptr<Texture>>>{nullptr},
		const std::array<float, 4>& tex_smpl_coords = std::array<float, 4>{0.0, 0.0, 1.0, 1.0});

	RenderedObject(const RenderedObject& other);
	~RenderedObject();

	void move_to(const std::array<float, 2>& bottomleft);
	void set_velocity(const std::array<float, 2>& velocity);

	void set_textures(std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures);
	const std::shared_ptr<std::vector<std::unique_ptr<Texture>>>& get_textures() const
	{
		return m_textures;
	}

	void set_current_texture(int current_texture);
	const Texture& get_current_texture() const;

	const bool get_use_textures() const { return m_use_textures; }

	void set_angle(float angle) { m_angle = angle;	}
	const float get_angle() const { return m_angle; }
	
	const float get_width() const { return m_width; }
	const float get_height() const { return m_height; }

	const int get_uid() const { return m_uid; }

	void set_color(const std::array<float, 4>& color) { m_color = color; }
	const std::array<float, 4>& get_color() const { return m_color; }

	const std::array<float, 2>& get_bottomleft() const { return m_bottomleft; }

	const std::array<float, 4>& get_tex_smpl_coords() const { return m_tex_smpl_coords; }

	void register_renderer(std::shared_ptr<Renderer>& renderer) { m_renderer = renderer; }

	void gradient_move(float time_elapsed);
};
