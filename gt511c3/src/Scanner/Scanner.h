#pragma once

#include <string>
#include <unordered_map>
#include <serial/serial.h>

#include "Protocol.h"

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
	std::unordered_map<std::string, std::string> get_ports_list();

	void open(int flags);
	void close();
	void add_user(int flags);
	int get_users_count();
	bool user_exists(int flags);
	bool delete_user(int flags);
	bool delete_all_users();
	bool toggle_led(int flags);
	bool is_finger_pressed();
	bool change_baud_rate(int flags);
	unsigned char* get_image();
	Texture* get_raw_image();

	inline void set_port(const std::string& port) {
		int toto = 4;
		this->port = port;
	}

	inline DeviceInfoPacket* get_device_infos() { return device_infos; }

	template<typename T>
	size_t send_command(Command command, int param = 0);

	bool receive_ack(int* param = nullptr);
	int receive_data(unsigned char* data, int length);

private:
	std::unique_ptr<serial::Serial> serial;
	DeviceInfoPacket* device_infos;
	std::string port;
	unsigned int baud_rate;
	unsigned int timeout;
	bool debug;
};

