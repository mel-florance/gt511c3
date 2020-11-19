#pragma once

#include <string>
#include <iostream>
#include <array>

#include "Application/Controller.h"
#include "Interface/TextureAtlas.h"

class IconsController : public Controller {
public:
	IconsController();
	~IconsController() = default;

	void OnEvent(Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;

	enum class IconType {
		IMAGE,
		BUTTON
	};

	void drawIcon(
		const std::string& name,
		const std::array<float, 2>& size,
		IconType type = IconType::IMAGE,
		const ImVec4& tint = ImVec4(1, 1, 1, 1)
	);

private:
	std::unique_ptr<TextureAtlas> atlas;
};
