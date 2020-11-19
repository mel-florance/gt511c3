#pragma once

#include <array>

#include "Application/Controller.h"

class ToolbarController : public Controller {
public:
	ToolbarController();

	void OnEvent(Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;
};
