#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Core/Utils.h"
#include <serial/serial.h>

#define CSV_DELIMITER ';'

#include "Application/Controller.h"

struct Device {
	std::string port;
	std::string name;
	unsigned int baud_rate = 9600;
	serial::bytesize_t databits = serial::bytesize_t::eightbits;
	serial::parity_t parity = serial::parity_t::parity_none;
	serial::flowcontrol_t flow_control = serial::flowcontrol_t::flowcontrol_none;
	serial::stopbits_t stopbits = serial::stopbits_t::stopbits_one;
	unsigned short device_id = 0;
	unsigned int users_count = 0;
	unsigned long firmware_version;
	unsigned long iso_area_max_size;
	std::string serial_number;
};

class DevicesController : public Controller {
public:
	DevicesController();

	void OnEvent(Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;

	Device* add_device(const Device& device);
	void remove_device(size_t index);
	void update_device(size_t index, const Device& data);
	std::vector<Device>& load_devices(const std::string& filename);
	void save_devices(const std::string& filename);

	inline std::vector<Device>& get_devices() { return devices; }
	inline void set_devices(std::vector<Device>& devices) { this->devices = devices; }

	Device* current_device;
	std::string current_name;
	std::vector<Device> devices;
};
