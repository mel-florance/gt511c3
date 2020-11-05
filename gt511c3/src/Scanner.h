#pragma once

#include <string>

#include <serial/serial.h>
struct DeviceInfoPacket;
class Texture;

class Scanner
{
public:
	Scanner();
	~Scanner();

	bool connect();
	void disconnect();
	void test_transmission();
	std::vector<std::string> get_ports_list();

	void open(int flags);
	void close();
	void add_user(int flags);
	void toggle_led(int flags);
	bool is_finger_pressed();
	bool change_baud_rate(int flags);
	unsigned char* get_image();
	Texture* get_raw_image();

	inline void set_port(const std::string& port) { this->port = port; }
	inline DeviceInfoPacket* get_device_infos() { return device_infos; }

private:
	std::unique_ptr<serial::Serial> serial;
	DeviceInfoPacket* device_infos;
	std::string port;
	unsigned int baud_rate;
	unsigned int timeout;
};

