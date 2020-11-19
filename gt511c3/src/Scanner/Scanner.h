#pragma once

#include <string>
#include <mutex>
#include <atomic>
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
	std::unordered_map<std::string, std::string> get_ports_list();

	void open(int flags);
	void close();
	int add_user(int flags, int quality = 0);
	int get_users_count();
	bool user_exists(int flags);
	bool delete_user(int flags);
	bool delete_all_users();
	bool verify_user(int flags);
	int identify_user();
	bool toggle_led(int flags);
	bool is_finger_pressed();
	bool capture_finger(int flags);
	bool change_baud_rate(int flags);
	unsigned char* get_image();
	Texture* get_raw_image(const std::string& filename, std::atomic<float>& progress);

	inline void set_port(const std::string& port) {
		this->port = port;
	}

	inline DeviceInfoPacket* get_device_infos() { return device_infos; }

	template<typename T>
	size_t send_command(Command command, int param = 0);

	bool receive_ack(int* param = nullptr);
	int receive_data(unsigned char* data, int length);

	static std::mutex mutex;

	inline unsigned int getBaudRate() { return serial->getBaudrate();  }
	inline serial::bytesize_t getDatabits() { return serial->getBytesize();  }
	inline serial::parity_t getParity() { return serial->getParity();  }
	inline serial::stopbits_t getStopbits() { return serial->getStopbits();  }
	inline serial::flowcontrol_t getFlowcontrol() { return serial->getFlowcontrol();  }

	inline bool getCD() { return serial->getCD(); }
	inline bool getDSR() { return serial->getDSR(); }
	inline bool getCTS() { return serial->getCTS(); }
	inline bool getRI() { return serial->getRI(); }

	inline size_t getTX() { return tx; }
	inline size_t getRX() { return rx; }

private:
	serial::Serial* serial;
	DeviceInfoPacket* device_infos;
	std::string port;
	unsigned int baud_rate;
	unsigned int timeout;
	bool debug;

	size_t tx;
	size_t rx;
};

