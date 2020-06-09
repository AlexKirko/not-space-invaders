#pragma once

#include <array>
#include <stdexcept>
#include <vector>
#include <memory>

// Implements functionality that all rendered objects should have
class RenderedObject
{
private:
	std::array<double, 2> m_center;
	std::array<double, 2> m_velocity;
	const bool m_movable;
	int m_bitmap_ind;
	double m_angle;
	// Vector of shared pointers to bitmaps
	// Bitmaps are vectors of unsigned char of length 3 * width * height
	const std::vector<std::shared_ptr<std::vector<unsigned char>>> m_bitmaps;
	// Bitmap width and height are included for ease of access
	const int m_width;
	const int m_height;
	// Cached bitmap
	bool m_use_cached_bitmap;
	std::unique_ptr<std::vector<unsigned char>> m_cached_bitmap;

public:
	void move_to(const std::array<double, 2>& center);
	void set_velocity(const std::array<double, 2>& velocity);

	void set_cached_bitmap(const std::vector<unsigned char>& cached_bitmap);

	void set_bitmap_ind(int bitmap_ind) { m_bitmap_ind = bitmap_ind; }
	const int get_bitmap_ind() const { return m_bitmap_ind; }

	void set_angle(double angle) { m_angle = angle;	}
	const double get_angle() const { return m_angle; }

	const std::array<double, 2>& get_center() const { return m_center; }
};
