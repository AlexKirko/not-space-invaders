#pragma once

#include "RenderedObject.h"

class Alien : public RenderedObject
{
public:
	Alien(const std::array<float, 2>& bottomleft,
		int width, int height,
		bool movable, bool use_textures,
		float angle,
		const std::array<float, 2>& velocity,
		const std::array<float, 4>& color,
		std::shared_ptr<std::vector<std::unique_ptr<Texture>>> textures) :
			RenderedObject{ bottomleft,
				width, height,
				movable, use_textures,
				angle,
				velocity,
				color,
				textures }
	{}
};