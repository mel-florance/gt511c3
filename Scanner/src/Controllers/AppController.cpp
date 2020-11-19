#include "AppController.h"
#include "Application/Application.h"
#include "Interface/ImGuiExtensions.h"
#include "Controllers/MenuController.h"
#include "Controllers/ToolbarController.h"
#include "Controllers/ScannerController.h"
#include "Controllers/UsersController.h"
#include "Interface/Texture.h"

AppController::AppController() :
	Controller(),
	show_modal_devices_manager(false),
	show_modal_add_user(false),
	show_modal_parameters(false),
	modal_user_name(""),
	image_download_progress(0.0f),
	current_image(nullptr)
{
}

void AppController::OnEvent(Event& event)
{
}

void AppController::OnUpdate(float delta)
{
	if (image_download_progress == 100.0f) {
		Utils::platform_sleep(100);

		Texture* image = new Texture(
			"./data/image.bmp",
			false,
			false,
			Texture::ChannelType::RGB,
			true
		);

		image->load();
		image_download_progress = 0.0f;
		current_image = &(*image);
	}
}

void AppController::OnRender()
{
	auto menu = Application::Get().get_controller<MenuController>("menu");
	auto toolbar = Application::Get().get_controller<ToolbarController>("toolbar");
	auto scanner = Application::Get().get_controller<ScannerController>("scanner");
	auto users = Application::Get().get_controller<UsersController>("users");

	if (menu != nullptr) {
		menu->ui->render();
	}

	if (show_modal_parameters) {
		ImGui::SetNextWindowCentered(ImVec2(700.0f, 400.0f));
		ImGui::Begin("Parameters", &show_modal_parameters);
		ImGui::End();
	}
	if (show_modal_add_user) {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
		ImGui::Begin("Add user", &show_modal_add_user);
		ImGui::PopStyleVar();

		ImGui::Columns(2, "add_user_form", false);
		ImGui::SetColumnWidth(0, 100);

		ImGui::Text("Name");
		ImGui::NextColumn();
		ImGui::InputText("##add_user_name", modal_user_name, sizeof(modal_user_name));

		ImGui::Columns(1);

		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 45.0f);

		if (ImGui::Button("create", ImVec2(-1, 25.0f))) {
			auto id = scanner->get()->add_user(1);

			if (id != -1 && strlen(modal_user_name) > 0)
			{
				std::thread thread([&]() {
					std::cout << "Starting thread..." << std::endl;
					scanner->get()->get_raw_image(std::to_string(id), image_download_progress);
				});

				thread.detach();

				User user;
				user.id = id;
				user.name = modal_user_name;
				users->add_user(user);
				users->save_users("./data/users.csv");
				users->set_table_data();
			}
		}

		ImGui::End();
	}



}
