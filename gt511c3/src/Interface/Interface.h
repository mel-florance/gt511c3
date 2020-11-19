#pragma once

#include "imgui.h"
#include "imgui_internal.h"

class Controller;

class Interface {
public:
	Interface();

	virtual void render() {};

	Controller* controller;
};
