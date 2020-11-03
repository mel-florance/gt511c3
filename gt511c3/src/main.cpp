#include <Windows.h>
#include "Scanner.h"

void blink_led(Scanner& scanner) {

	while (true) {
		scanner.toggle_led(1);
		Sleep(1000);
		scanner.toggle_led(0);
		Sleep(1000);
	}
}

int main(int argc, char** argv)
{
	Scanner scanner;
	//scanner.list_ports();

	scanner.open(1);
	scanner.toggle_led(1);
	Sleep(1000);
	//scanner.adduser();
	scanner.close();
	//blink_led(scanner);
}
