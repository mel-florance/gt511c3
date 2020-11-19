#include "ToolbarInterface.h"
#include "Application/Application.h"

#include "Controllers/IconsController.h"
#include "Controllers/AppController.h"
#include "Controllers/ScannerController.h"
#include "Controllers/DevicesController.h"

ToolbarInterface::ToolbarInterface()
{
}

void ToolbarInterface::render()
{
	ImGuiWindowFlags win_flags = 0
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoScrollbar;

	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

	ImGui::SetNextWindowClass(&window_class);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.0f, 4.0f));
	ImGui::Begin("Toolbar", NULL, win_flags);
	ImGui::PopStyleVar();

	std::array<float, 2> icon_size = { 25.0f, 25.0f };
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

	auto icons = Application::Get().get_controller<IconsController>("icons");
	auto app = Application::Get().get_controller<AppController>("app");
	auto scanner = Application::Get().get_controller<ScannerController>("scanner");
	auto devices = Application::Get().get_controller<DevicesController>("devices");

	icons->drawIcon("joystick", icon_size, IconsController::IconType::BUTTON);
	if (ImGui::IsItemClicked(0))
		app->show_modal_devices_manager = !app->show_modal_devices_manager;

	ImGui::SameLine();
	icons->drawIcon("gear", icon_size, IconsController::IconType::BUTTON);
	if (ImGui::IsItemClicked(0))
		app->show_modal_parameters = !app->show_modal_parameters;

	ImGui::SameLine();
	icons->drawIcon("photo", icon_size, IconsController::IconType::BUTTON);
	if (ImGui::IsItemClicked(0)) {}

	ImGui::SameLine();

	icons->drawIcon(
		scanner->cmos_led_status ? "bulb_on" : "bulb_off",
		icon_size,
		IconsController::IconType::BUTTON
	);

	if (ImGui::IsItemClicked(0) && scanner->serial_connected) {
		scanner->cmos_led_status = !scanner->cmos_led_status;
		scanner->get()->toggle_led((int)scanner->cmos_led_status);
	}

	ImGui::PopStyleVar();

	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.0f);
	ImGui::SetCursorPosY(4.0f);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5.5f);

	if (ImGui::BeginCombo("##devices_list", devices->current_name.c_str(), 0)) {

		for (auto& device : devices->devices) {
			bool is_selected = device.name == devices->current_name;

			if (ImGui::Selectable(device.name.c_str(), is_selected)) {
				devices->current_device = &device;
				devices->current_name = device.name;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.0f);
	ImGui::SetCursorPosY(4.0f);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5.5f);

	const char* connected_txt = scanner->serial_connected
		? "Disconnect"
		: "Connect";

	if (ImGui::Button(connected_txt)) {
		if (scanner->serial_connected) {
			scanner->get()->disconnect();
			scanner->serial_connected = false;
			scanner->cmos_led_status = false;
		}
		else if (devices->current_device != nullptr) {
			scanner->get()->set_port(devices->current_device->port); // TODO REPLACE CURRENT SELECTED DEVICE PORT
			scanner->serial_connected = scanner->get()->connect();

			//if (scanner->serial_connected) {
			//	scanner->cmos_led_status = true;

			//	device_infos = scanner->get_device_infos();
			//	users_count = scanner->get_users_count();

			//	std::cout << "Loading existing users..." << std::endl;

			//	Device device;
			//	device.baud_rate = std::atoi(baud_rate);
			//	device.name = ports_list[selected_port];
			//	device.device_id = DEVICE_ID;
			//	device.firmware_version = std::atoi(int_to_hex(device_infos->firmware_version).c_str());
			//	device.iso_area_max_size = device_infos->iso_area_max_size;
			//	device.port = selected_port;

			//	size_t serial_size = sizeof device_infos->serial_number / sizeof device_infos->serial_number[0];

			//	std::stringstream str;
			//	for (size_t i = 0; i < serial_size; ++i)
			//		str << HEX(device_infos->serial_number[i]);

			//	device.serial_number = str.str();
			//	device.users_count = users_count;

			//	/*	devices_manager->add_device(device);
			//		devices_manager->save_devices("./data/devices.csv");*/
			//}
		}
	}

	ImGui::End();
}
