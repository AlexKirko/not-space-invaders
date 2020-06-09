#include "RenderedString.h"

RenderedString::RenderedString(
	const std::string& str, 
	const std::array<float, 2>& bottomleft, float lett_height, float lett_w_to_h,
	std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures
) :
RenderedObject{
	bottomleft,
	static_cast<int>(lett_height * lett_w_to_h * str.length()), static_cast<int>(lett_height),
	false
},
m_lett_height{ lett_height },
m_lett_w_to_h{ lett_w_to_h }
{
	set_textures(textures);
	set_current_texture(0);

	float lett_width{ m_lett_height * lett_w_to_h };

	// Now we build the vector of letters that will be the ones getting rendered
	for (int i{ 0 }; i < str.length(); i++)
	{
		std::array<float, 2> bottomleft{ get_bottomleft()[0] + i * lett_width, get_bottomleft()[1] };

		// Sampling coordinates depend on the char code
		// We support ASCII from 32 till 95
		int to_render{ static_cast<int>(str.at(i)) };
		float row{ static_cast<float>(to_render / 8 - 4) };
		float col{ static_cast<float>(to_render % 8) };
		// Warning: hardcoded to our particular 8x8 font texture
		std::array<float, 4> tex_smpl_coords{
			//0.0, 0.0, 1.0, 1.0
			0.125 * col,
			1 - 0.125 * (row + 0.5),
			0.125 * col + lett_w_to_h / 16.0,
			1 - 0.125 * row,
		};
		
		std::unique_ptr<RenderedObject> r_letter = std::make_unique<RenderedObject>(
			bottomleft,
			// Extra pixel to offset rounding down
			static_cast<int>(lett_width) + 1, static_cast<int>(m_lett_height) + 1,
			false,
			true,
			0.0,
			std::array<float, 2>{0.0, 0.0},
			std::array<float, 4>{1.0, 1.0, 1.0, 1.0},
			get_textures(),
			tex_smpl_coords
		);
		r_letter->set_current_texture(0);
		m_r_letters.push_back(std::move(r_letter));
	}

}
