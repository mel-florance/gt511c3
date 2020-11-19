#pragma once

#include "Events/Event.h"
#include "Interface/Interface.h"

class Controller
{
public:
	Controller();

	virtual void OnEvent(Event& event) {};
	virtual void OnUpdate(float delta) {};
	virtual void OnRender() {};

	Interface* ui;
	int state;
	bool active = true;
};
