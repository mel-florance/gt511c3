#pragma once

#include "Application/Application.h"
#include "Interface/ImGuiLayer.h"
#include "Scanner/Scanner.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

class ScannerApplication : public Application
{
public:
	ScannerApplication();
	~ScannerApplication();
};

class ApplicationLayer : public Layer
{
public:
	ApplicationLayer(Engine* engine);
	~ApplicationLayer();

	void OnUpdate(float delta);
	void OnAttach();
	void OnImGuiRender();
	void OnEvent(Event& event);

	Engine* engine;
	float led_timer;
	bool is_led_blinking;
	int led_status;
	bool is_finger_pressed;
	bool watch_finger;
	float finger_timer;
	int finger_status;
	char baud_rate[7];
	bool led_on;
	bool device_opened;
	std::string port;
	bool serial_connected;

	DeviceInfoPacket* device_infos;
	std::vector<std::string> ports_list;
	const char* selected_port;
private:
	std::unique_ptr<Scanner> scanner;
};
