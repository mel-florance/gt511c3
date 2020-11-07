#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Core/Utils.h"

struct Device {
	std::string port;
	std::string description;
	unsigned int baud_rate = 9600;
	unsigned short device_id = 0;
	unsigned int users_count = 0;
	unsigned long firmware_version;
	unsigned long iso_area_max_size;
	std::string serial_number;
};

class DevicesManager
{
public:
	~DevicesManager();

	inline Device* add_device(const Device& device) {
		auto exists = std::find_if(devices.begin(), devices.end(), [=](const Device& item) {
			return item.port == device.port;
		});

		if (exists == devices.end()) {
			return &devices.emplace_back(std::move(device));
		}

		return nullptr;
	}

	inline void remove_device(size_t index) {
		devices.erase(devices.begin() + index);
	}

	inline void update_device(size_t index, const Device& data) {
		if (index >= 0 && index < devices.size() && devices.size() > 0) {
			auto device = devices.at(index);
			device = std::move(data);
		}
	}

	inline std::vector<Device>& load_devices(const std::string& filename) {
		std::ifstream file;
		file.open(filename);
		std::string line;
		size_t index = 0;

		while (std::getline(file, line)) {
			if (index > 1) {
				auto parts = Utils::splitString(line, ",");

				Device device;
				device.baud_rate = std::atoi(parts[0].c_str());
				device.description = parts[1];
				device.device_id = std::atoi(parts[2].c_str());
				device.port = parts[3];
				device.users_count = std::atoi(parts[4].c_str());
				device.firmware_version = std::atoi(parts[5].c_str());
				device.iso_area_max_size = std::atoi(parts[6].c_str());
				device.serial_number = parts[7];

				devices.emplace_back(std::move(device));
			}

			++index;
		}

		file.close();

		return devices;
	}

	inline void save_devices(const std::string& filename) {
		std::ofstream file;
		file.open(filename, std::ios::out | std::ios::trunc);

		file
			<< "Baud rate,"
			<< "Description,"
			<< "Device ID,"
			<< "Port,"
			<< "Users count,"
			<< "Firmware version,"
			<< "ISO area max size,"
			<< "Serial number\n";

		for (auto& device : devices) {
			file
				<< device.baud_rate << ","
				<< device.description << ","
				<< device.device_id << ","
				<< device.port << ","
				<< device.users_count << ","
				<< device.firmware_version << ","
				<< device.iso_area_max_size << ","
				<< device.serial_number << "\n";
		}

		file.close();
	}

private:
	std::vector<Device> devices;
};

