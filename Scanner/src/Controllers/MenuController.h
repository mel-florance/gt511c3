#pragma once

#include "Application/Controller.h"
#include "../Interfaces/MenuInterface.h"

class MenuController : public Controller {
public:
	MenuController();

	void OnEvent(Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;
};
