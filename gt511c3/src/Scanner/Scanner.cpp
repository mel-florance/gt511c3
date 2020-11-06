#include "Scanner.h"
#include <Windows.h>
#include "Interface/Texture.h"
#include <string>
#include <iostream>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Scanner::Scanner() :
	serial(nullptr),
	port("COM5"),
	baud_rate(9600),
	timeout(1000),
	debug(true)
{
}

Scanner::~Scanner()
{
	serial->close();
}

bool Scanner::connect()
{
	try {
		serial.reset(new serial::Serial(
			port,
			baud_rate,
			serial::Timeout::simpleTimeout(timeout)
		));
	}
	catch (std::exception e) {
		std::cout << "Error while opening port " << port << std::endl;
		std::cout << e.what() << std::endl << std::endl;
	}

	bool opened = serial->isOpen();

	if (opened) {
		open(1);
		Sleep(100);
		toggle_led(1);
	}

	return opened;
}

void Scanner::disconnect()
{
	close();
	delete device_infos;
	serial.reset(nullptr);
}

void Scanner::test_transmission()
{
	int count = 0;
	std::string payload = "test";

	std::cout << "Testing transmission..." << std::endl;

	while (count < 10) {
		size_t written = serial->write(payload);
		std::string response = serial->read(payload.length() + 1);

		std::cout << "Bytes written: " << written << ", "
			      << "Bytes read: " << response.length() << ", "
				  << "Response: " << response << std::endl;

		++count;
	}

	std::cout << "End of transmission" << std::endl;
}

std::vector<std::string> Scanner::get_ports_list()
{
	auto devices = serial::list_ports();
	auto it = devices.begin();
	std::vector<std::string> out = {};

	while (it != devices.end()) {
		serial::PortInfo device = *it++;
		out.push_back(device.port);
	}

	return out;
}

void Scanner::open(int flags)
{
	this->send<CommandPacket>(Command::OPEN, flags);

	auto response = this->receive<ResponsePacket>();

	if (response != nullptr) {
		if (response->command_code == Command::ACK && flags) {
			auto infos = this->receive<DeviceInfoPacket>(sizeof(DeviceInfoPacket) + 6, 4);

			if (infos != nullptr) {
				device_infos = infos;
			}
		}
	}
}

void Scanner::close() 
{
	toggle_led(0);
	this->send<CommandPacket>(Command::CLOSE);
}

void Scanner::add_user(int flags) {
	std::cout << "-----------------------" << std::endl;

	toggle_led(1);
	Sleep(100);
}

void Scanner::toggle_led(int flags)
{
	this->send<CommandPacket>(Command::CMOS_LED, flags);
}

bool Scanner::is_finger_pressed()
{
	this->send<CommandPacket>(Command::IS_PRESS_FINGER);

	auto response = this->receive<ResponsePacket>();

	if (response != nullptr) {
		if (response->command_code == Command::ACK) {
			return response->parameter == 0;
		}
	}

	return false;
}

bool Scanner::change_baud_rate(int flags)
{
	this->send<CommandPacket>(Command::CHANGE_BAUD_RATE, flags);

	auto response = this->receive<ResponsePacket>();

	if (response != nullptr) {
		if (response->command_code == Command::ACK) {
			std::cout << "Baud rate changed  to: " << flags << std::endl;
			return true;
		}
	}

	return false;
}

unsigned char* Scanner::get_image()
{
	std::cout << "-----------------------" << std::endl;

	CommandPacket packet;
	packet.start_code1 = START_CODE1;
	packet.start_code2 = START_CODE2;
	packet.device_id = DEVICE_ID;
	packet.parameter = 1;
	packet.command_code = Command::GET_IMAGE;
	packet.checksum = calc_packet_checksum<CommandPacket>(&packet);

	auto d = reinterpret_cast<unsigned char*>(&packet);

	std::cout << "Bytes sent: " << std::endl;

	for (int i = 0; i < PACKET_SIZE; i++) {
		std::cout << HEX(d[i]) << " ";
	}

	std::cout << std::endl;

	size_t w = serial->write(d, PACKET_SIZE);
	std::cout << "Bytes written: " << std::dec << w << std::endl;

	Sleep(100);

	uint8_t* readBuf = new uint8_t[PACKET_SIZE];
	auto r = serial->read(readBuf, PACKET_SIZE);

	std::cout << "Bytes read: " << std::dec << r << std::endl;

	auto res = reinterpret_cast<ResponsePacket*>(readBuf);

	if (res->command_code == Command::ACK) {
		std::cout << "Acknowledge packet received" << std::endl;
		std::cout << "Getting image..." << std::endl;

		Sleep(100);
		change_baud_rate(115200);
		Sleep(100);

		uint8_t* readBuf = new uint8_t[PACKET_IMAGE_SIZE];
		auto r2 = serial->read(readBuf, PACKET_IMAGE_SIZE);

		std::cout << "Bytes read: " << std::dec << r2 << std::endl;
	}
	else {
		std::cout << "Error: " <<
			get_error_code(res->command_code) << " (" << HEX(res->command_code) << ")" <<
			std::endl;
	}

	return nullptr;
}


Texture* Scanner::get_raw_image()
{
	std::cout << "-----------------------" << std::endl;

	CommandPacket packet;
	packet.start_code1 = START_CODE1;
	packet.start_code2 = START_CODE2;
	packet.device_id = DEVICE_ID;
	packet.parameter = 1;
	packet.command_code = Command::GET_RAW_IMAGE;
	packet.checksum = calc_packet_checksum<CommandPacket>(&packet);

	auto d = reinterpret_cast<unsigned char*>(&packet);

	std::cout << "Bytes sent: " << std::endl;

	for (int i = 0; i < PACKET_SIZE; i++) {
		std::cout << HEX(d[i]) << " ";
	}

	std::cout << std::endl;
	Sleep(100);
	size_t w = serial->write(d, PACKET_SIZE);
	std::cout << "Bytes written: " << std::dec << w << std::endl;

	Sleep(100);

	uint8_t* readBuf = new uint8_t[PACKET_SIZE];
	auto r = serial->read(readBuf, PACKET_SIZE);
	std::cout << "Bytes read: " << std::dec << r << std::endl;

	auto res = reinterpret_cast<ResponsePacket*>(readBuf);

	Sleep(100);

	if (res->command_code == Command::ACK) {
		std::cout << "Acknowledge packet received" << std::endl;
		std::cout << "Getting raw image..." << std::endl;

		unsigned char gbyImgRaw[320 * 240];
		unsigned char gbyImgRaw2[240 * 320 / 4];
		serial->setTimeout(10000, 0, 0, 100, 0);
		auto r2 = serial->read(gbyImgRaw2, sizeof gbyImgRaw2);
		std::cout << "Image bytes read: " << std::dec << r2 << std::endl;

		std::memset(gbyImgRaw, 0, sizeof gbyImgRaw);

		int i, j;
		for (i = 0; i < 120; i++)
		{
			for (j = 0; j < 160; j++)
			{
				gbyImgRaw[320 * (2 * i + 0) + (2 * j + 0)] = gbyImgRaw2[i * 160 + j];
				gbyImgRaw[320 * (2 * i + 0) + (2 * j + 1)] = gbyImgRaw2[i * 160 + j];
				gbyImgRaw[320 * (2 * i + 1) + (2 * j + 0)] = gbyImgRaw2[i * 160 + j];
				gbyImgRaw[320 * (2 * i + 1) + (2 * j + 1)] = gbyImgRaw2[i * 160 + j];
			}
		}

		Texture* image = new Texture("raw");
		image->load_from_memory = true;
		image->load(gbyImgRaw);
	}
	else {
		std::cout << "Error: " <<
			get_error_code(res->command_code) << " (" << HEX(res->command_code) << ")" <<
			std::endl;
	}

	return nullptr;
}

template<typename T>
size_t Scanner::send(Command command, int flags)
{
	std::cout << "-----------------------" << std::endl;
	Sleep(50);

	unsigned char* packet = create_packet<T>(flags, command);
	size_t bytes = serial->write(packet, PACKET_SIZE);

	if (debug) {
		for (int i = 0; i < PACKET_SIZE; i++)
			std::cout << HEX(packet[i]) << " ";

		std::cout << std::endl;
		std::cout << "Bytes written: " << std::dec << bytes << std::endl;
	}

	return bytes;
}

template<typename T>
T* Scanner::receive(size_t length, int offset)
{
	Sleep(50);

	uint8_t* buffer = new uint8_t[length];
	size_t bytes = serial->read(buffer, length);

	uint8_t* data = new uint8_t[length];
	std::memcpy(data, buffer + offset, sizeof T);

	auto response = reinterpret_cast<T*>(data);

	if (debug) {
		std::cout << "Bytes read: " << std::dec << bytes << std::endl;

		if (response == nullptr) {
			std::cout << "Error while decoding packet." << std::endl;
		}
	}

	return response;
}
