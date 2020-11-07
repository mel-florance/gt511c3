#include "ScannerApplication.h"
#include <string>
#include <sstream>

#ifdef GT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "Scanner/Protocol.h"
#include "Scanner/Scanner.h"
#include "Interface/Texture.h";

ScannerApplication::ScannerApplication() : Application(false)
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
	led_on(true),
	device_opened(false),
	device_infos(nullptr),
	serial_connected(false),
	ports_list({}),
	selected_port(""),
	users_count(0),
	user_to_delete(0),
	devices_manager(nullptr)
{
	scanner = std::make_unique<Scanner>();
	devices_manager = std::make_unique<DevicesManager>();
	fingerprint_icon = std::make_unique<Texture>("./data/fingerprint.png");
	fingerprint_icon->load();
	fingerprint_icon_highlight = std::make_unique<Texture>("./data/fingerprint_highlight.png");
	fingerprint_icon_highlight->load();

	auto devices = devices_manager->load_devices("./data/devices.csv");

	for (auto& device : devices) {
		std::cout << "Device " << device.port << std::endl;
	}

	ports_list = scanner->get_ports_list();
}

ApplicationLayer::~ApplicationLayer()
{
	scanner->toggle_led(0);
	Sleep(100);
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
}

void ApplicationLayer::OnAttach()
{
}

void ApplicationLayer::OnEvent(Event& event) {

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
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_DockNodeHost |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("ApplicationLayer", &p_open, window_flags);
	ImGui::PopStyleVar();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::BeginChild("menus");

	ImGui::SetNextItemWidth(150);
	if (ImGui::BeginCombo("##ports_list", selected_port, 0))
	{
		for (auto it = ports_list.begin(); it != ports_list.end(); ++it) {
			bool is_selected = std::string(selected_port) == it->first;
			auto p = it->first + std::string(" - " + it->second);

			if (ImGui::Selectable(p.c_str(), is_selected))
				selected_port = it->first.c_str();

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	//if (ImGui::IsItemClicked()) {
	//	ports_list = scanner->get_ports_list();
	//	selected_port = "";
	//}

	const char* connected_txt = serial_connected ? "Disconnect" : "Connect";
	if (ImGui::Button(std::string(connected_txt).c_str(), ImVec2(150, 25))) {
		if (serial_connected) {
			scanner->disconnect();
			serial_connected = false;
			led_on = true;
		}
		else {
			scanner->set_port(selected_port);
			serial_connected = scanner->connect();

			if (serial_connected) {
				device_infos = scanner->get_device_infos();

				Device device;
				device.baud_rate = std::atoi(baud_rate);
				device.description = ports_list[selected_port];
				device.device_id = DEVICE_ID;
				device.firmware_version = std::atoi(int_to_hex(device_infos->firmware_version).c_str());
				device.iso_area_max_size = device_infos->iso_area_max_size;
				device.port = selected_port;

				size_t serial_size = sizeof device_infos->serial_number / sizeof device_infos->serial_number[0];

				std::stringstream str;
				for (int i = 0; i < serial_size; ++i)
					str << HEX(device_infos->serial_number[i]);

				device.serial_number = str.str();
				device.users_count = users_count;

				devices_manager->add_device(device);
				devices_manager->save_devices("./data/devices.csv");
			}
		}
	}

	if (serial_connected) {
		if (device_infos != nullptr) {
			ImGui::Text((std::string("Firmware version: ") + int_to_hex(device_infos->firmware_version)).c_str());

			size_t serial_size = sizeof device_infos->serial_number / sizeof device_infos->serial_number[0];

			std::stringstream str;
			for (int i = 0; i < serial_size; ++i)
				str << HEX(device_infos->serial_number[i]);

			ImGui::Text((std::string("Serial number: ") + str.str()).c_str());
			ImGui::Text((std::string("ISO Area Max Size: ") + std::to_string(device_infos->iso_area_max_size)).c_str());
		}


		ImGui::Text(is_finger_pressed ? "Pressed" : "Not pressed");
		ImGui::Text((std::string("Users count: ") + std::to_string(users_count)).c_str());
		ImGui::SetNextItemWidth(150);
		ImGui::InputText("##Baud rate", baud_rate, 7);

		if (ImGui::Button("Set baud rate", ImVec2(150, 25))) {
			scanner->change_baud_rate(std::stoi(baud_rate));
		}

		if (ImGui::Button("Get image", ImVec2(150, 25))) {
			scanner->get_image();
		}

		if (ImGui::Button("Get raw image", ImVec2(150, 25))) {
			scanner->get_raw_image();
		}

		if (ImGui::Button("Toggle led", ImVec2(150, 25))) {
			led_on = !led_on;
			scanner->toggle_led((int)led_on);
		}

		if (ImGui::Button("Get users count", ImVec2(150, 25))) {
			users_count = scanner->get_users_count();
		}

		ImGui::SetNextItemWidth(150);
		ImGui::DragInt("Id to delete: ", &user_to_delete, 1.0f, 0, 199);

		if (ImGui::Button("Delete user", ImVec2(150, 25))) {
			if (scanner->delete_user(user_to_delete)) {
				std::cout << "User #" << user_to_delete << " deleted." << std::endl;
			}
		}

		if (ImGui::Button("Delete all", ImVec2(150, 25))) {
			if (scanner->delete_all_users()) {
				std::cout << "All users deleted" << std::endl;
			}
		}

		
	}

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 p = ImGui::GetCursorScreenPos();
	static float sz = 128.0f;
	static float thickness = 4.0f;
	static ImVec4 col = ImVec4();
	void* tex = nullptr;
	if (led_on)
		col = ImVec4(0.3f, 0.8f, 0.9f, 1.0f);
	else
		col = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

	if (is_finger_pressed)
		tex = (void*)fingerprint_icon_highlight->getId();
	else
		tex = (void*)fingerprint_icon->getId();

	const ImU32 col32 = ImColor(col);
	float x = p.x + 4.0f, y = p.y + 4.0f, spacing = 8.0f;

	ImVec2 finger_pos = ImVec2(x + sz * 0.5f, y + sz * 0.5f);
	draw_list->AddCircle(finger_pos, sz * 0.5f, col32, 64, thickness);
	ImGui::SetCursorScreenPos(ImVec2(finger_pos.x - 36.5f, finger_pos.y - 49.5f));

	ImGui::Image(tex, ImVec2(75.0f, 99.0f));

	ImGui::EndChild();
	ImGui::End();
}
