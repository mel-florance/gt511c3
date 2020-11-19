#pragma once

#include <unordered_map>
#include <atomic>
#include <vector>
#include <tuple>
#include <time.h>

#include "Application/Application.h"

#include "Interface/ImGuiLayer.h"
#include "Interface/ImGuiExtensions.h"
#include "Scanner/Scanner.h"
#include "IconsManager.h"
#include "Core/Utils.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "Application/Controller.h"

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

	void display_menu_bar();

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
	unsigned int users_count;
	int user_to_delete;
	DeviceInfoPacket* device_infos;
	std::unordered_map<std::string, std::string> ports_list;
	const char* selected_port;
	std::unique_ptr<Texture> fingerprint_icon;
	std::unique_ptr<Texture> fingerprint_icon_highlight;
	Texture* current_image;
	std::atomic<float> image_download_progress;
	bool show_modal_devices_manager;
	bool show_modal_add_user;
	bool show_modal_parameters;
	const char* selected_device;

	char modal_user_name[20];
	ImGui::Table users_table;

	std::vector<std::tuple<tm, int, const char*>> log;

private:
	std::unique_ptr<Scanner> scanner;
	std::unique_ptr<IconsManager> icons_manager;
};
