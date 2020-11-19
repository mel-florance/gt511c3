#include "ScannerApplication.h"

#include <string>
#include <sstream>
#include <thread>

#ifdef GT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "Interface/Texture.h"
#include "Scanner/Protocol.h"
#include "Scanner/Scanner.h"

#include "Controllers/AppController.h"
#include "Controllers/MenuController.h"
#include "Controllers/IconsController.h"
#include "Controllers/ToolbarController.h"
#include "Controllers/ScannerController.h"
#include "Controllers/UsersController.h"
#include "Controllers/DevicesController.h"
#include "Controllers/DetailsController.h"
#include "Controllers/LoggerController.h"
#include "Controllers/StatusbarController.h"

ScannerApplication::ScannerApplication() :
	Application(false, WindowProps("FingerPrint Scanner"))
{
	Engine* engine = this->getEngine();

	auto layer = new ApplicationLayer(engine);
	auto main = new ImGuiLayer();
	PushLayer(layer);
	PushOverlay(main);

	this->GetWindow().SetVSync(true);
}

ScannerApplication::~ScannerApplication()
{
}

ApplicationLayer::ApplicationLayer(Engine* engine) :
	Layer("Test"),
	engine(engine),
	is_led_blinking(false),
	led_timer(0.0f),
	led_status(1),
	is_finger_pressed(false),
	watch_finger(false),
	finger_timer(0.0f),
	finger_status(1),
	baud_rate("9600"),
	led_on(false),
	device_opened(false),
	device_infos(nullptr),
	serial_connected(false),
	ports_list({}),
	selected_port(""),
	users_count(0),
	user_to_delete(0),
	current_image(nullptr),
	image_download_progress(0.0f),
	show_modal_devices_manager(false),
	show_modal_add_user(false),
	selected_device(""),
	show_modal_parameters(false),
	users_table(ImGui::Table("users_table"))
{
	auto& app = Application::Get();
	app.register_controller<AppController>("app");
	app.register_controller<MenuController>("menu");
	app.register_controller<IconsController>("icons");
	app.register_controller<ToolbarController>("toolbar");
	app.register_controller<ScannerController>("scanner");
	app.register_controller<UsersController>("users");
	app.register_controller<DetailsController>("details");
	app.register_controller<DevicesController>("devices");
	app.register_controller<LoggerController>("logger");
	app.register_controller<StatusbarController>("statusbar");

	scanner = std::make_unique<Scanner>();
	icons_manager = std::make_unique<IconsManager>();

	users_table.set_headers({"ID", "Name" });

	fingerprint_icon = std::make_unique<Texture>("./data/fingerprint.png");
	fingerprint_icon->load();
	fingerprint_icon_highlight = std::make_unique<Texture>("./data/fingerprint_highlight.png");
	fingerprint_icon_highlight->load();
}

ApplicationLayer::~ApplicationLayer()
{
	scanner->toggle_led(0);
	Utils::platform_sleep(100);
	scanner->close();
}

void ApplicationLayer::OnUpdate(float delta)
{
	if (is_led_blinking && serial_connected) {
		led_timer += delta;

		if (led_timer >= 1.0f && led_status == 1) {
			scanner->toggle_led(1);
			led_status = 0;
		}
		else if (led_timer >= 2.0f && led_status == 0) {
			scanner->toggle_led(0);
			led_timer = 0.0f;
			led_status = 1;
		}
	}

	if (watch_finger && serial_connected) {
		finger_timer += delta;

		if (finger_timer >= 0.0f && finger_status == 1) {
			finger_status = 0;
		}
		else if (finger_timer >= 0.3f && finger_status == 0) {
			is_finger_pressed = scanner->is_finger_pressed();
			finger_timer = 0.0f;
			finger_status = 1;
		}
	}

	Application::Get().controllers_update(delta);
}

void ApplicationLayer::OnAttach()
{
}

void ApplicationLayer::OnEvent(Event& event) {
	Application::Get().controllers_event(event);
}

void ApplicationLayer::display_menu_bar()
{
	
}

void ApplicationLayer::OnImGuiRender()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	bool p_open;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_DockNodeHost |
		ImGuiWindowFlags_MenuBar;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("ApplicationLayer", &p_open, window_flags);
	ImGui::PopStyleVar();

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MainDock");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	Application::Get().controllers_render();

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	//ImGui::Begin("Details");

	//ImGui::SetNextItemWidth(150);

	//
	//if (ImGui::BeginCombo("##ports_list", selected_port, 0))
	//{
	//	for (auto it = ports_list.begin(); it != ports_list.end(); ++it) {
	//		bool is_selected = std::string(selected_port) == it->first;
	//		auto p = it->first + std::string(" - " + it->second);

	//		if (ImGui::Selectable(p.c_str(), is_selected))
	//			selected_port = it->first.c_str();

	//		if (is_selected)
	//			ImGui::SetItemDefaultFocus();
	//	}

	//	ImGui::EndCombo();
	//}

	//if (ImGui::IsItemClicked()) {
	//	ports_list = scanner->get_ports_list();
	//	selected_port = "";
	//}

	//const char* connected_txt = serial_connected ? "Disconnect" : "Connect";

	////if (ImGui::Button(std::string(connected_txt).c_str(), ImVec2(150, 25))) {
	////	if (serial_connected) {
	////		scanner->disconnect();
	////		serial_connected = false;
	////		led_on = false;
	////	}
	////	else {
	////		scanner->set_port(selected_port);
	////		serial_connected = scanner->connect();

	////		if (serial_connected) {
	////			led_on = true;

	////			device_infos = scanner->get_device_infos();
	////			users_count = scanner->get_users_count();

	////			std::cout << "Loading existing users..." << std::endl;

	////			Device device;
	////			device.baud_rate = std::atoi(baud_rate);
	////			device.name = ports_list[selected_port];
	////			device.device_id = DEVICE_ID;
	////			device.firmware_version = std::atoi(int_to_hex(device_infos->firmware_version).c_str());
	////			device.iso_area_max_size = device_infos->iso_area_max_size;
	////			device.port = selected_port;

	////			size_t serial_size = sizeof device_infos->serial_number / sizeof device_infos->serial_number[0];

	////			std::stringstream str;
	////			for (size_t i = 0; i < serial_size; ++i)
	////				str << HEX(device_infos->serial_number[i]);

	////			device.serial_number = str.str();
	////			device.users_count = users_count;

	////		/*	devices_manager->add_device(device);
	////			devices_manager->save_devices("./data/devices.csv");*/
	////		}
	////	}
	////}

	//if (serial_connected) {
	//	if (device_infos != nullptr) {
	//		ImGui::Text((std::string("Firmware version: ") + int_to_hex(device_infos->firmware_version)).c_str());

	//		size_t serial_size = sizeof device_infos->serial_number / sizeof device_infos->serial_number[0];

	//		std::stringstream str;
	//		for (int i = 0; i < serial_size; ++i)
	//			str << HEX(device_infos->serial_number[i]);

	//		ImGui::Text((std::string("Serial number: ") + str.str()).c_str());
	//		ImGui::Text((std::string("ISO Area Max Size: ") + std::to_string(device_infos->iso_area_max_size)).c_str());
	//	}

	//	ImGui::Text(is_finger_pressed ? "Pressed" : "Not pressed");
	//
	//	if (ImGui::Button("Get image", ImVec2(150.0f, 25.0f))) {
	//		scanner->get_image();
	//	}

	//	if (ImGui::Button("Get raw image", ImVec2(150.0f, 25.0f))) {

	//		std::thread thread([&]() {
	//			std::cout << "Starting thread..." << std::endl;
	//			scanner->get_raw_image(image_download_progress);
	//		});

	//		thread.detach();
	//	}
	//}

	//const ImVec2 p = ImGui::GetCursorScreenPos();
	//static float sz = 128.0f;
	//static float thickness = 4.0f;
	//static ImVec4 col = ImVec4();
	//unsigned int tex = 0;

	//if (led_on)
	//	col = ImVec4(0.3f, 0.8f, 0.9f, 1.0f);
	//else
	//	col = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

	//if (is_finger_pressed || image_download_progress > 0.0f)
	//	tex = fingerprint_icon_highlight->getId();
	//else
	//	tex = fingerprint_icon->getId();

	//const ImU32 col32 = ImColor(col);
	//float x = p.x + 4.0f, y = p.y + 4.0f, spacing = 8.0f;

	//ImVec2 finger_pos = ImVec2(x + sz * 0.5f, y + sz * 0.5f);
	//draw_list->AddCircle(finger_pos, sz * 0.5f, col32, 64, thickness);
	//ImGui::SetCursorScreenPos(ImVec2(finger_pos.x - 36.5f, finger_pos.y - 49.5f));

	//ImGui::Image((void*)(uintptr_t)tex, ImVec2(75.0f, 99.0f));

	////std::lock_guard<std::mutex> lock(Scanner::mutex);

	//if (image_download_progress == 100.0f) {
	//	Utils::platform_sleep(100);

	//	Texture* image = new Texture(
	//		"./data/image.bmp",
	//		false,
	//		false,
	//		Texture::ChannelType::RGB,
	//		true
	//	);

	//	image->load();
	//	image_download_progress = 0.0f;
	//	current_image = &(*image);
	//}

	//if (current_image != nullptr) {
	//	ImGui::SetCursorPosX(300.0f);
	//	ImGui::SetCursorPosY(20.0f);

	//	ImGui::Image(
	//		(void*)(uintptr_t)current_image->getId(),
	//		ImVec2(320.0f * 2.0f, 240.0f * 2.0f),
	//		ImVec2(0.04f, 0.0f),
	//		ImVec2(1.0f, 1.0f)
	//	);
	//}

	//if (image_download_progress > 0.0f)
	//	ImGui::ProgressBar(image_download_progress / 100.0f);

	//ImGui::End();

	if (show_modal_devices_manager) {
		/*auto& devices = devices_manager->get_devices();

		ImGui::SetNextWindowCentered(ImVec2(700.0f, 400.0f));
		ImGui::Begin("Devices Manager", &show_modal_devices_manager);
		ImGui::Columns(2, "devices_table");

		float max = 0;
		for (auto& device : devices) {
			auto size = ImGui::CalcTextSize(device.name.c_str());
			if (size.x > max)
				max = size.x;
		}

		ImGui::SetColumnWidth(0, max + 45.0f);

		ImGui::BeginChild("devices_list");
	
		ImGui::SetCursorPosY(5);

		ImGui::Text("DEVICES");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (20.0f + 4.0f));
		icons_manager->drawIcon("plus", { 20.0f, 20.0f }, IconsManager::IconType::BUTTON);
		if (ImGui::IsItemClicked(0)) {
			show_modal_add_user = true;
		}

		ImGui::Separator();

		for (auto& device : devices) {
			auto selected = selected_device == device.name.c_str();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 10));
			if (ImGui::Selectable((' ' + device.name).c_str(), selected)) {
				selected_device = device.name.c_str();
			}
			ImGui::PopStyleVar();
		}

		ImGui::EndChild();
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::BeginChild("devices_infos");
		ImGui::PopStyleVar();

		ImGui::Columns(2, "device_infos", false);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::SetCursorPosY(8.0f);

		for (auto& device : devices) {

			if (selected_device == device.name.c_str()) {

				ImGui::Text("INFORMATIONS");
				ImGui::Dummy(ImVec2(0, 8));
				ImGui::NextColumn();
				ImGui::NextColumn();

				ImGui::Dummy(ImVec2(0, 0));
				ImGui::Text("Port");
				ImGui::NextColumn();

				if (ImGui::BeginCombo("##ports_list", device.port.c_str(), 0)) {

					for (auto it = ports_list.begin(); it != ports_list.end(); ++it) {
						bool is_selected = device.port == it->first;
						auto p = it->first + std::string(" - " + it->second);

						if (ImGui::Selectable(p.c_str(), is_selected))
							device.port = it->first.c_str();

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (ImGui::IsItemClicked()) {
					ports_list = scanner->get_ports_list();
					selected_port = "";
				}

				ImGui::NextColumn();
				ImGui::Dummy(ImVec2(0, 0));
				ImGui::Text("Baudrate");
				ImGui::NextColumn();

				ImGui::InputText("##Baudrate", baud_rate, 7);

				ImGui::NextColumn();
				ImGui::Dummy(ImVec2(0, 0));
				ImGui::Text("Databits");
				ImGui::NextColumn();

				if (ImGui::BeginCombo("##databits_list", std::to_string((int)device.databits).c_str(), 0)) {

					for (auto& bit : databits_list) {
						bool selected = device.databits == (serial::bytesize_t)bit;

						if (ImGui::Selectable(std::to_string((int)bit).c_str(), selected))
							device.databits = (serial::bytesize_t)bit;

						if (selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::NextColumn();
				ImGui::Dummy(ImVec2(0, 0));
				ImGui::Text("Parity");
				ImGui::NextColumn();

				if (ImGui::BeginCombo("##parity_list", parity_to_str(device.parity), 0)) {

					for (auto it = parity_list.begin(); it != parity_list.end(); ++it) {
						bool is_selected = device.parity == it->first;

						if (ImGui::Selectable(it->second, is_selected))
							device.parity = it->first;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::NextColumn();
				ImGui::Dummy(ImVec2(0, 0));
				ImGui::Text("Flow control");
				ImGui::NextColumn();

				if (ImGui::BeginCombo("##flowcontrol_list", flowcontrol_to_str(device.flow_control), 0)) {

					for (auto it = flowcontrol_list.begin(); it != flowcontrol_list.end(); ++it) {
						bool is_selected = device.flow_control == it->first;

						if (ImGui::Selectable(it->second, is_selected))
							device.flow_control = it->first;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::NextColumn();
				ImGui::Dummy(ImVec2(0, 0));
				ImGui::Text("Stopbits");
				ImGui::NextColumn();

				if (ImGui::BeginCombo("##stopbits_list", stopbits_to_str(device.stopbits), 0)) {

					for (auto it = stopbits_list.begin(); it != stopbits_list.end(); ++it) {
						bool is_selected = device.stopbits == it->first;

						if (ImGui::Selectable(it->second, is_selected))
							device.stopbits = it->first;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			}
		}

		ImGui::Columns(1);
		ImGui::EndChild();

		ImGui::Columns(1);
		ImGui::End();*/
	}

	ImGui::End();
}
