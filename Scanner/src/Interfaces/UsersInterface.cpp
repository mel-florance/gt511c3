#include "UsersInterface.h"
#include "Controllers/ScannerController.h"
#include "Controllers/IconsController.h"
#include "Controllers/AppController.h"
#include "Controllers/UsersController.h"
#include "Application/Application.h"

UsersInterface::UsersInterface()
{
}

void UsersInterface::render()
{
	auto scanner = Application::Get().get_controller<ScannerController>("scanner");
	auto icons = Application::Get().get_controller<IconsController>("icons");
	auto app = Application::Get().get_controller<AppController>("app");
	auto users = Application::Get().get_controller<UsersController>("users");

	ImGui::Begin("Users");

	if (scanner->serial_connected) {
		char users_count_str[10];
		sprintf(users_count_str, "%d / %d", users->get_users().size(), 200);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.0f);
		ImGui::Text(users_count_str);

		auto style = ImGui::GetStyle();
		float offset_scrollbar = 0;

		if (ImGui::GetScrollMaxY() > 0)
			offset_scrollbar = style.ScrollbarSize;

		ImGui::SetCursorPosY(33.0f);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (65.0f + offset_scrollbar));
		icons->drawIcon("cross", { 25.0f, 25.0f }, IconsController::IconType::BUTTON);
		if (ImGui::IsItemClicked(0)) {
			if (scanner->get()->delete_all_users()) {
				users->clear_users();
				users->save_users("./data/users.csv");
				users->set_table_data();
				std::cout << "All users deleted" << std::endl;
			}
		}

		ImGui::SetCursorPosY(33.0f);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (33.0f + offset_scrollbar));
		icons->drawIcon("plus", { 25.0f, 25.0f }, IconsController::IconType::BUTTON);
		if (ImGui::IsItemClicked(0)) {
			app->show_modal_add_user = true;
		}

		auto& table = users->get_table();
		table.set_draw_list(ImGui::GetWindowDrawList());
		table.set_margin_top(45);
		table.draw();
	}

	ImGui::End();
}
