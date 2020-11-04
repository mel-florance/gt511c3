#pragma once

#include "Application.h"
#include "ImGuiLayer.h"
#include "Scanner.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

class ScannerApplication : public Application
{
public:
	ScannerApplication();
	~ScannerApplication();

	void blink_led();

private:
	std::unique_ptr<Scanner> scanner;
};

class ApplicationLayer : public Layer
{
public:
	ApplicationLayer(Engine* engine);

	void OnUpdate(float delta);
	void OnAttach();
	void OnImGuiRender();
	void OnEvent(Event& event);
	
	Engine* engine;
};
