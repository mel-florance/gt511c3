#include "ScannerController.h"

const std::array<int, 4> ScannerController::databits_list = {
	5, 6, 7, 8
};

const std::unordered_map<serial::parity_t, const char*> ScannerController::parity_list = {
	{ serial::parity_t::parity_none, "None" },
	{ serial::parity_t::parity_even, "Even" },
	{ serial::parity_t::parity_mark, "Mark" },
	{ serial::parity_t::parity_odd, "Odd" },
	{ serial::parity_t::parity_space, "Space" }
};

const std::unordered_map<serial::flowcontrol_t, const char*> ScannerController::flowcontrol_list = {
	{ serial::flowcontrol_t::flowcontrol_none, "None" },
	{ serial::flowcontrol_t::flowcontrol_hardware, "Hardware" },
	{ serial::flowcontrol_t::flowcontrol_software, "Software" }
};

const std::unordered_map<serial::stopbits_t, const char*> ScannerController::stopbits_list = {
	{ serial::stopbits_t::stopbits_one, "1" },
	{ serial::stopbits_t::stopbits_one_point_five, "1.5" },
	{ serial::stopbits_t::stopbits_two, "2" }
};

ScannerController::ScannerController() :
	Controller(),
	serial_connected(false),
	cmos_led_status(false)
{
	scanner = std::make_shared<Scanner>();

	std::cout << "Finding opened ports..." << std::endl;
	ports = scanner->get_ports_list();
}

void ScannerController::OnEvent(Event& event)
{
}

void ScannerController::OnUpdate(float delta)
{
}

void ScannerController::OnRender()
{
}

std::string ScannerController::parity_to_str(serial::parity_t parity)
{
	switch (parity) {
	default:
	case serial::parity_none: return "None";
	case serial::parity_even: return "Even";
	case serial::parity_mark: return "Mark";
	case serial::parity_odd: return "Odd";
	case serial::parity_space: return "Space";
	}
}

std::string ScannerController::flowcontrol_to_str(serial::flowcontrol_t flowcontrol)
{
	switch (flowcontrol) {
	default:
	case serial::flowcontrol_none: return "Off";
	case serial::flowcontrol_software: return "Software";
	case serial::flowcontrol_hardware: return "Hardware";
	}
}

std::string ScannerController::stopbits_to_str(serial::stopbits_t parity)
{
	switch (parity) {
	default:
	case serial::stopbits_one: return "1";
	case serial::stopbits_one_point_five: return "1.5";
	case serial::stopbits_two: return "2";
	}
}
