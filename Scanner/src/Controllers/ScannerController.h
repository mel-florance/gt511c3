#pragma once

#include <array>
#include "Application/Controller.h"
#include "Scanner/Scanner.h"

class ScannerController : public Controller {
public:
	ScannerController();

	void OnEvent(Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;

	std::string parity_to_str(serial::parity_t parity);
	std::string flowcontrol_to_str(serial::flowcontrol_t flowcontrol);
	std::string stopbits_to_str(serial::stopbits_t parity);

	inline std::shared_ptr<Scanner> get() { return scanner; }

	bool serial_connected;
	bool cmos_led_status;

	std::shared_ptr<Scanner> scanner;
	std::unordered_map<std::string, std::string> ports;

	static const std::array<int, 4> databits_list;
	static const std::unordered_map<serial::parity_t, const char*> parity_list;
	static const std::unordered_map<serial::flowcontrol_t, const char*> flowcontrol_list;
	static const std::unordered_map<serial::stopbits_t, const char*> stopbits_list;
};
