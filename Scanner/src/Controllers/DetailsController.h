#pragma once

#include "Application/Controller.h"
#include "Interfaces/DetailsInterface.h"

class DetailsController : public Controller {
public:
	DetailsController();

	void OnEvent(Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;
};
