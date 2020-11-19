#pragma once

#include "Application/Controller.h"
#include "../Interfaces/LoggerInterface.h"

class LoggerController : public Controller {
public:
	LoggerController();

	void OnEvent(Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;
};
