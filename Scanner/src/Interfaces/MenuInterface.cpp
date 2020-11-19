#include "MenuInterface.h"
#include "Application/Application.h"
#include "Controllers/AppController.h"
#include "Controllers/UsersController.h"
#include "Controllers/LoggerController.h"
#include "Controllers/ToolbarController.h"
#include "Controllers/StatusbarController.h"
#include "Controllers/DetailsController.h"

MenuInterface::MenuInterface()
{
}

void MenuInterface::render()
{
	if (ImGui::BeginMainMenuBar())
	{
		auto app = Application::Get().get_controller<AppController>("app");
		auto users = Application::Get().get_controller<UsersController>("users");
		auto logger = Application::Get().get_controller<LoggerController>("logger");
		auto toolbar = Application::Get().get_controller<ToolbarController>("toolbar");
		auto statusbar = Application::Get().get_controller<StatusbarController>("statusbar");
		auto details = Application::Get().get_controller<DetailsController>("details");

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Devices Manager...", "CTRL+D")) {
				app->show_modal_devices_manager = !app->show_modal_devices_manager;
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "CTRL+Q")) {
				Application::Get().close();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Parameters...", "CTRL+P")) {
				app->show_modal_parameters = !app->show_modal_parameters;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Users", "CTRL+U", users->active))
				users->active = !users->active;
			if (ImGui::MenuItem("Log", "CTRL+L", logger->active))
				logger->active = !logger->active;
			if (ImGui::MenuItem("Details", "CTRL+D", details->active))
				details->active = !details->active;
			if (ImGui::MenuItem("Toolbar", "CTRL+T", toolbar->active))
				toolbar->active = !toolbar->active;
			if (ImGui::MenuItem("Status", "CTRL+S", statusbar->active))
				statusbar->active = !statusbar->active;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Find updates...")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Manual")) {}
			if (ImGui::MenuItem("Report a bug...")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("About...")) {}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
