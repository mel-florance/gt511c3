#pragma once

#include <string>
#include <iostream>
#include <array>

class TextureAtlas;

class IconsManager
{
public:
	IconsManager();
	~IconsManager();

	enum class IconType {
		IMAGE,
		BUTTON
	};

	void drawIcon(
		const std::string& name,
		const std::array<float, 2>& size,
		IconType type = IconType::IMAGE
	);

private:
	std::unique_ptr<TextureAtlas> atlas;
};
