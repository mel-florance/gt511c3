#include "ScannerApplication.h"
#include <string>
#include <sstream>

#ifdef GT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "Protocol.h"

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
	led_on(false),
	device_opened(false),
	device_infos(nullptr),
	serial_connected(false),
	ports_list({}),
	selected_port(nullptr)
{
	scanner = std::make_unique<Scanner>();
}

ApplicationLayer::~ApplicationLayer()
{
	scanner->toggle_led(0);
	Sleep(100);
	scanner->close();
}

void ApplicationLayer::OnUpdate(float delta)
{
	if (is_led_blinking) {
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

	if (watch_finger) {
		finger_timer += delta;

		if (finger_timer >= 0.2f && finger_status == 1) {
			finger_status = 0;
		}
		else if (finger_timer >= 0.4f && finger_status == 0) {
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
		for (int n = 0; n < ports_list.size(); n++)
		{
			auto port = ports_list.at(n).c_str();
			bool is_selected = (selected_port == port);

			if (ImGui::Selectable(port, is_selected))
				selected_port = port;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	if (ImGui::IsItemClicked()) {
		ports_list = scanner->get_ports_list();
	}

	const char* connected_txt = serial_connected ? "Disconnect" : "Connect";
	if (ImGui::Button(std::string(connected_txt).c_str(), ImVec2(150, 25))) {
		if (serial_connected) {
			scanner->disconnect();
			serial_connected = false;
		}
		else {
			scanner->set_port(selected_port);
			serial_connected = scanner->connect();

			if (serial_connected) {
				device_infos = scanner->get_device_infos();
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
	}

	ImGui::EndChild();
	ImGui::End();
}
