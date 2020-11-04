#include <iostream>
#include <Windows.h>
#include <gt511c3.h>

void blink_led(Scanner& scanner) {
	scanner.open(0);
	Sleep(100);

	while (true) {
		scanner.toggle_led(1);
		Sleep(1000);
		scanner.toggle_led(0);
		Sleep(1000);
	}

	scanner.close();
}

int main(int argc, char** argv) {
	Scanner scanner;
	scanner.open(1);

	//blink_led(scanner);
}
