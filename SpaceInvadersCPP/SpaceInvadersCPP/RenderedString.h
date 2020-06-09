#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "RenderedObject.h"

/*
This object implements rendering a string on screen
Currently, only latin letters and numbers are supported
(and simple punctuation signs)
*/
class RenderedString : public RenderedObject
{
private:
	// Letters we'll be rendering
	std::vector<std::unique_ptr<RenderedObject>> m_r_letters;
	// Letter height in pixels
	float m_lett_height;
	// Letter width-to-height aspect ratio
	float m_lett_w_to_h;
public:
	RenderedString(
		const std::string& str,
		const std::array<float, 2>& bottomleft,
		float lett_height,
		float lett_w_to_h,
		// The font texture to sample from
		std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures
	);

	// Rendering currently isn't a const operation
	// TODO: see if we can fix this
	std::vector<std::unique_ptr<RenderedObject>>& get_letters()
	{ return m_r_letters; }
};