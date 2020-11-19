#pragma once

#include <unordered_map>
#include <array>

#include "Texture.h"

class TextureAtlas : public Texture
{
public:
	TextureAtlas(const std::string& filename, bool mipmaps = false, bool flipped = true);
	~TextureAtlas();

	inline void addItem(const std::string& name, unsigned int x, unsigned int y)
	{
		items_coords[name] = { x, y };
		items_positions[name] = calculateGridPos(x, y);
	}

	inline const std::array<unsigned int, 2> getItemCoords(const std::string& name) const
	{
		auto item = items_coords.find(name);

		if (item != items_coords.end())
			return item->second;

		return std::array<unsigned int, 2>();
	}

	inline const std::array<float, 4> getItemPosition(const std::string& name) const
	{
		auto item = items_positions.find(name);

		if (item != items_positions.end())
			return item->second;

		return std::array<float, 4>();
	}

private:
	inline const std::array<float, 4> calculateGridPos(unsigned int x, unsigned int y)
	{
		int w = getWidth(), 
			h = getHeight();

		std::array<float, 2> pos = {
			size[0] * y,
			size[1] * x
		};

		pos[0] += offset[0] * y;
		pos[1] += offset[1] * x;

		return {
			pos[0] / w,
			pos[1] / h,
			(pos[0] + size[0]) / w,
			(pos[1] + size[1]) / h
		};
	}

	std::array<float, 2> offset;
	std::array<float, 2> size;

	std::unordered_map<std::string, std::array<unsigned int, 2>> items_coords;
	std::unordered_map<std::string, std::array<float, 4>> items_positions;
};
