#include "DevicesController.h"
#include "Application/Application.h"
#include "Interface/ImGuiExtensions.h"
#include "Controllers/ScannerController.h"

DevicesController::DevicesController() :
	Controller(),
	current_device(nullptr),
	current_name("")
{
	std::cout << "Loading existing devices..." << std::endl;
	load_devices("./data/devices.csv");
}

void DevicesController::OnEvent(Event& event)
{
}

void DevicesController::OnUpdate(float delta)
{
}

void DevicesController::OnRender()
{
	
}


Device* DevicesController::add_device(const Device& device) {
	auto exists = std::find_if(devices.begin(), devices.end(), [=](const Device& item) {
		return item.port == device.port;
		});

	if (exists == devices.end()) {
		return &devices.emplace_back(std::move(device));
	}

	return nullptr;
}

void DevicesController::remove_device(size_t index) {
	devices.erase(devices.begin() + index);
}

void DevicesController::update_device(size_t index, const Device& data) {
	if (index >= 0 && index < devices.size() && devices.size() > 0) {
		auto device = devices.at(index);
		device = std::move(data);
	}
}

std::vector<Device>& DevicesController::load_devices(const std::string& filename) {
	std::ifstream file;
	file.open(filename);
	std::string line;
	size_t index = 0;

	std::cout << "Loading from file: " << filename << std::endl;

	while (std::getline(file, line)) {
		if (index > 0) {
			auto parts = Utils::splitString(line, std::string(1, CSV_DELIMITER));

			Device device;
			device.baud_rate = std::atoi(parts[0].c_str());
			device.name = parts[1];
			device.device_id = std::atoi(parts[2].c_str());
			device.port = parts[3];
			device.users_count = std::atoi(parts[4].c_str());
			device.firmware_version = std::atoi(parts[5].c_str());
			device.iso_area_max_size = std::atoi(parts[6].c_str());
			device.serial_number = parts[7];
			device.databits = (serial::bytesize_t)std::atoi(parts[8].c_str());
			device.parity = (serial::parity_t)std::atoi(parts[9].c_str());
			device.flow_control = (serial::flowcontrol_t)std::atoi(parts[10].c_str());
			device.stopbits = (serial::stopbits_t)std::atoi(parts[11].c_str());

			std::cout << "Loaded device " << device.device_id << std::endl;
			devices.push_back(device);
		}

		++index;
	}

	file.close();

	return devices;
}

void DevicesController::save_devices(const std::string& filename) {
	std::ofstream file;
	file.open(filename, std::ios::out | std::ios::trunc);

	file
		<< "Baud rate" << CSV_DELIMITER
		<< "Name" << CSV_DELIMITER
		<< "Device ID" << CSV_DELIMITER
		<< "Port" << CSV_DELIMITER
		<< "Users count" << CSV_DELIMITER
		<< "Firmware version" << CSV_DELIMITER
		<< "ISO area max size" << CSV_DELIMITER
		<< "Serial number" << CSV_DELIMITER
		<< "Databits" << CSV_DELIMITER
		<< "Parity" << CSV_DELIMITER
		<< "Flow control" << CSV_DELIMITER
		<< "Stopbits\n";

	for (auto& device : devices) {
		file
			<< device.baud_rate << CSV_DELIMITER
			<< device.name << CSV_DELIMITER
			<< device.device_id << CSV_DELIMITER
			<< device.port << CSV_DELIMITER
			<< device.users_count << CSV_DELIMITER
			<< device.firmware_version << CSV_DELIMITER
			<< device.iso_area_max_size << CSV_DELIMITER
			<< device.serial_number << CSV_DELIMITER
			<< device.databits << CSV_DELIMITER
			<< device.parity << CSV_DELIMITER
			<< device.flow_control << CSV_DELIMITER
			<< device.stopbits;
	}

	file.close();
}

