#pragma once

#include <atomic>

#include "Application/Controller.h"

class Texture;

class AppController : public Controller {
public:
	AppController();

	void OnEvent(Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;

	bool show_modal_devices_manager;
	bool show_modal_add_user;
	bool show_modal_parameters;

	char modal_user_name[20];
	std::atomic<float> image_download_progress;
	Texture* current_image;
};
