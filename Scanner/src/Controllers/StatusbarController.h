#pragma once

#include "Application/Controller.h"
#include "../Interfaces/StatusbarInterface.h"

class StatusbarController : public Controller {
public:
	StatusbarController();

	void OnEvent(Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;
};
