#include "Scanner.h"
#include "Protocol.h"
#include <iomanip>
#include <Windows.h>

#define HEX(x) \
	std::setw(2) << std::setfill('0') << std::hex << int(x)

Scanner::Scanner() :
	serial(nullptr),
	port("COM5"),
	baud_rate(9600),
	timeout(0)
{
	try {
		serial = new serial::Serial(
			port,
			baud_rate, 
			serial::Timeout::simpleTimeout(timeout)
		);
	}
	catch (std::exception e) {
		std::cout << "Error while opening port " << port << std::endl;
		std::cout << e.what() << std::endl << std::endl;
	}

	std::cout << "Serial port " << port << " is " << 
		(serial->isOpen() ? "opened" : "closed") << std::endl;
}

Scanner::~Scanner()
{
	serial->close();
	delete serial;
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

void Scanner::list_ports()
{
	auto devices = serial::list_ports();
	auto it = devices.begin();

	while (it != devices.end()) {
		serial::PortInfo device = *it++;

		std::cout << device.port << ", " 
				  << device.description << ", "
			      << device.hardware_id << std::endl;
	}
}

void Scanner::open(int flags)
{
	unsigned char buffer[PACKET_SIZE];

	CommandPacket packet;
	packet.start_code1 = START_CODE1;
	packet.start_code2 = START_CODE2;
	packet.device_id = DEVICE_ID;
	packet.parameter = flags;
	packet.command_code = Command::OPEN;
	packet.checksum = calc_packet_checksum<CommandPacket>(&packet);

	auto d = reinterpret_cast<unsigned char*>(&packet);

	std::cout << "Bytes sent: " << std::endl;

	for (int i = 0; i < PACKET_SIZE; i++) {
		std::cout << HEX(int(d[i])) << " ";
	}

	std::cout << std::endl;

	size_t w = serial->write(d, PACKET_SIZE);
	std::cout << "Bytes written: " << std::dec << w << std::endl;

	Sleep(50);

	uint8_t* readBuf = new uint8_t[PACKET_SIZE];
	auto r = serial->read(readBuf, PACKET_SIZE);

	std::cout << "Bytes read: " << std::dec << r << std::endl;

	auto res = reinterpret_cast<ResponsePacket*>(readBuf);

	if (res->response == Command::ACK) {
		std::cout << "Acknowledge packet received" << std::endl;
	}
	else {
		std::cout << "Error: " <<
			get_error_code(res->response) << " (" << res->response << ")" <<
		std::endl;
	}

	if (flags) {
		Sleep(50);
		auto len = sizeof(DeviceInfoPacket) + 6;
		uint8_t* readBufData = new uint8_t[len];

		auto r2 = serial->read(readBufData, len);
		std::cout << "Bytes read: " << std::dec << r2 << std::endl;

		for (int i = 0; i < len; i++)
			std::cout << HEX(int(readBufData[i])) << " ";

		std::cout << std::endl;

		uint8_t* tmp = new uint8_t[len];
		std::memcpy(tmp, readBufData + 4, sizeof(DeviceInfoPacket));

		DeviceInfoPacket* info = reinterpret_cast<DeviceInfoPacket*>(tmp);

		std::cout << "Version:\t" << info->firmware_version << std::endl;
		std::cout << "ISO AMS:\t" << info->iso_area_max_size << std::endl;
		std::cout << "Serial number:\t";

		size_t serial_size = sizeof info->serial_number / sizeof info->serial_number[0];

		for (int i = 0; i < serial_size; ++i)
			std::cout << HEX(info->serial_number[i]);

		std::cout << std::endl;
	}
}

void Scanner::close() 
{
	toggle_led(0);
	Sleep(100);

	unsigned char buffer[PACKET_SIZE];

	CommandPacket packet;
	packet.start_code1 = START_CODE1;
	packet.start_code2 = START_CODE2;
	packet.device_id = DEVICE_ID;
	packet.parameter = 0;
	packet.command_code = Command::CLOSE;
	packet.checksum = calc_packet_checksum<CommandPacket>(&packet);

	auto d = reinterpret_cast<unsigned char*>(&packet);

	std::cout << "Bytes sent: " << std::endl;

	for (int i = 0; i < PACKET_SIZE; i++) {
		std::cout << HEX(int(d[i])) << " ";
	}

	std::cout << std::endl;

	size_t w = serial->write(d, PACKET_SIZE);
	std::cout << "Bytes written: " << std::dec << w << std::endl;
}

void Scanner::add_user(int flags) {
	toggle_led(1);
	Sleep(100);
}

void Scanner::toggle_led(int flags)
{
	unsigned char buffer[PACKET_SIZE];

	CommandPacket packet;
	packet.start_code1 = START_CODE1;
	packet.start_code2 = START_CODE2;
	packet.device_id = DEVICE_ID;
	packet.parameter = flags;
	packet.command_code = Command::CMOS_LED;
	packet.checksum = calc_packet_checksum<CommandPacket>(&packet);

	auto d = reinterpret_cast<unsigned char*>(&packet);

	std::cout << "Bytes sent: " << std::endl;

	for (int i = 0; i < PACKET_SIZE; i++) {
		std::cout << HEX(d[i]) << " ";
	}

	std::cout << std::endl;

	size_t w = serial->write(d, PACKET_SIZE);
	std::cout << "Bytes written: " << std::dec << w << std::endl;
}
