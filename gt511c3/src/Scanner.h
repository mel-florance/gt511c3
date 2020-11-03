#pragma once

#include <string>
#include <iostream>

#include <serial/serial.h>

class Scanner
{
public:
	Scanner();
	~Scanner();
	void test_transmission();
	void list_ports();

	void open(int flags);
	void close();
	void add_user(int flags);
	void toggle_led(int flags);

private:
	serial::Serial* serial;
	std::string port;
	unsigned int baud_rate;
	unsigned int timeout;
};

