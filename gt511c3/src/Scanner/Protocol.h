#pragma once

#include <climits>
#include <iomanip>

#define PACKET_SIZE 12
#define PACKET_HEADER_SIZE 2
#define PACKET_CHECKSUM_SIZE 2
#define PACKET_DEVICE_ID_SIZE 2

#define PACKET_IMAGE_SIZE 52116
#define PACKET_RAW_IMAGE_SIZE 19200

#define DEVICE_ID 0x0001
#define START_CODE1 0x55
#define START_CODE2 0xAA
#define START_CODE3 0x5A
#define START_CODE4 0xA5

#define HEX(x) \
	std::setw(2) << std::setfill('0') << std::uppercase << std::hex << int(x)

enum Command {
	OPEN                 = 0x01, // Initialization
	CLOSE                = 0x02, // Termination
	USB_INTERNAL_CHECK   = 0x03, // Check if the connected USB device is valid
	CHANGE_BAUD_RATE     = 0x04, // Change UART baud rate
	SET_IAP_MODE         = 0x05, // ENTER_IAP Mode (Enable firmware upgrade)
	CMOS_LED             = 0x12, // Control CMOS LED
	GET_ENROLL_COUNT     = 0x20, // get enrolled fingerprint count
	CHECK_ENROLLED       = 0x21, // Check if the specified id is already enrolled
	ENROLL_START         = 0x22, // Start an enrollment
	ENROLL_1             = 0x23, // Make 1st template for an enrollment
	ENROLL_2             = 0x24, // Make 2nd template for an enrollment
	ENROLL_3             = 0x25, // Make 3rd template for an enrollment, merge three template into one, save merge into db
	IS_PRESS_FINGER      = 0x26, // Check if a finger is placed on the sensor
	DELETE_ID            = 0x40, // Delete the fingerprint with the specified id
	DELETE_ALL           = 0x41, // Delete all fingerprints from the database
	VERIFY               = 0x50, // 1:1 verification of the capture fingerprint image with the specified id
	IDENTIFY             = 0x51, // 1:N identification of the capture fingerprint image with the database
	VERIFY_TEMPLATE      = 0x52, // 1:1 verification of a fingerprint template with the specified id
	IDENTIFY_TEMPLATE    = 0x53, // 1:N Identification of a fingerprint template with the database
	CAPTURE_FINGER       = 0x60, // Capture a fingerprint image (256x256) from the sensor
	MAKE_TEMPLATE        = 0x61, // Make template for the transmission
	GET_IMAGE            = 0x62, // Download the capture fingerprint image (256x256)
	GET_RAW_IMAGE        = 0x63, // Capture& download raw fingerprint image(320x240)
	GET_TEMPLATE         = 0x70, // Download the template of the specified id
	SET_TEMPLATE         = 0x71, // Upload the template of the specified id
	GET_DATABASE_START   = 0x72, // Start database download (obsolete)
	GET_DATABASE_END     = 0x73, // End database download (obsolete)
	UPGRADE_FIRMWARE     = 0x80, // Unsupported
	UPGRADE_ISO_CD_IMAGE = 0x81, // Unsupported
	ACK                  = 0x30, // Acknowledge
	NACK                 = 0x31  // Non-acknowledge
};

enum Error {
	NACK_TIMEOUT               = 0x1001, // Capture timeout (obsolete)
	NACK_INVALID_BAUD_RATE     = 0x1002, // Invalid serial baud rate (obsolete)
	NACK_INVALID_POS           = 0x1003, // The specified is not between 0 ~ 1999
	NACK_IS_NOT_USED           = 0x1004, // The specified id is not used
	NACK_IS_ALREADY_USED       = 0x1005, // The specified id is already used
	NACK_COMM_ERR              = 0x1006, // Communication error
	NACK_VERIFY_FAILED         = 0x1007, // 1:1 verification failure
	NACK_IDENTIFY_FAILED       = 0x1008, // 1:N identification failure
	NACK_DB_IS_FULL            = 0x1009, // The database is full
	NACK_DB_IS_EMPTY           = 0x100a, // The database is empty
	NACK_TURN_ERR              = 0x100b, // Invalid enrollment order (EnrollStart -> Enroll1 -> Enroll2 -> Enroll3) (obsolete)
	NACK_BAD_FINGER            = 0x100c, // Fingerprint too bad
	NACK_ENROLL_FAILED         = 0x100d, // Enrollment failure
	NACK_IS_NOT_SUPPORTED      = 0x100e, // The specified command isn't supported
	NACK_DEV_ERR               = 0x100f, // Device error, especially if Crypto-Chip is trouble
	NACK_CAPTURE_CANCELLED     = 0x1010, // The capturing is canceled (obsolete)
	NACK_INVALID_PARAM         = 0x1011, // Invalid parameter
	NACK_FINGER_IS_NOT_PRESSED = 0x1012  // Finger isn't pressed
};

template<typename T>
std::string int_to_hex(T i)
{
	std::stringstream stream;
	stream
		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::uppercase << std::hex << i;

	return stream.str();
}

static const char* get_error_code(int code) {
	switch (code) {
	default:								return "UNDEFINED";
	case Error::NACK_TIMEOUT:               return "NACK_TIMEOUT";
	case Error::NACK_INVALID_BAUD_RATE:     return "NACK_INVALID_BAUD_RATE";
	case Error::NACK_INVALID_POS:           return "NACK_INVALID_POS";
	case Error::NACK_IS_NOT_USED:           return "NACK_IS_NOT_USED";
	case Error::NACK_IS_ALREADY_USED:       return "NACK_IS_ALREADY_USED";
	case Error::NACK_COMM_ERR:              return "NACK_COMM_ERR";
	case Error::NACK_VERIFY_FAILED:         return "NACK_VERIFY_FAILED";
	case Error::NACK_IDENTIFY_FAILED:       return "NACK_IDENTIFY_FAILED";
	case Error::NACK_DB_IS_FULL:            return "NACK_DB_IS_FULL";
	case Error::NACK_DB_IS_EMPTY:           return "NACK_DB_IS_EMPTY";
	case Error::NACK_TURN_ERR:              return "NACK_TURN_ERR";
	case Error::NACK_BAD_FINGER:            return "NACK_BAD_FINGER";
	case Error::NACK_ENROLL_FAILED:         return "NACK_ENROLL_FAILED";
	case Error::NACK_IS_NOT_SUPPORTED:      return "NACK_IS_NOT_SUPPORTED";
	case Error::NACK_DEV_ERR:               return "NACK_DEV_ERR";
	case Error::NACK_CAPTURE_CANCELLED:     return "NACK_CAPTURE_CANCELLED";
	case Error::NACK_INVALID_PARAM:         return "NACK_INVALID_PARAM";
	case Error::NACK_FINGER_IS_NOT_PRESSED: return "NACK_FINGER_IS_NOT_PRESSED";
	}
}

static const char* command_to_string(Command command) {
	switch (command) {
	default: return "DATA";
	case Command::OPEN: return "OPEN";
	case Command::CLOSE: return "CLOSE";
	case Command::USB_INTERNAL_CHECK: return "USB_INTERNAL_CHECK";
	case Command::CHANGE_BAUD_RATE: return "CHANGE_BAUD_RATE";
	case Command::SET_IAP_MODE: return "SET_IAP_MODE";
	case Command::CMOS_LED: return "CMOS_LED";
	case Command::GET_ENROLL_COUNT: return "GET_ENROLL_COUNT";
	case Command::CHECK_ENROLLED: return "CHECK_ENROLLED";
	case Command::ENROLL_START: return "ENROLL_START";
	case Command::ENROLL_1: return "ENROLL_1";
	case Command::ENROLL_2: return "ENROLL_2";
	case Command::ENROLL_3: return "ENROLL_3";
	case Command::IS_PRESS_FINGER: return "IS_PRESS_FINGER";
	case Command::DELETE_ID: return "DELETE_ID";
	case Command::DELETE_ALL: return "DELETE_ALL";
	case Command::VERIFY: return "VERIFY";
	case Command::IDENTIFY: return "IDENTIFY";
	case Command::VERIFY_TEMPLATE: return "VERIFY_TEMPLATE";
	case Command::IDENTIFY_TEMPLATE: return "IDENTIFY_TEMPLATE";
	case Command::CAPTURE_FINGER: return "CAPTURE_FINGER";
	case Command::MAKE_TEMPLATE: return "MAKE_TEMPLATE";
	case Command::GET_IMAGE: return "GET_IMAGE";
	case Command::GET_RAW_IMAGE: return "GET_RAW_IMAGE";
	case Command::GET_TEMPLATE: return "GET_TEMPLATE";
	case Command::SET_TEMPLATE: return "SET_TEMPLATE";
	case Command::GET_DATABASE_START: return "GET_DATABASE_START";
	case Command::GET_DATABASE_END: return "GET_DATABASE_END";
	case Command::UPGRADE_FIRMWARE: return "UPGRADE_FIRMWARE";
	case Command::UPGRADE_ISO_CD_IMAGE: return "UPGRADE_ISO_CD_IMAGE";
	case Command::ACK: return "ACK";
	case Command::NACK: return "NACK";
	}
}

struct CommandPacket {
	unsigned char start_code1;
	unsigned char start_code2;
	unsigned short device_id;
	int parameter;
	unsigned short command_code;
	unsigned short checksum;
};

struct ResponsePacket {
	unsigned char start_code1;
	unsigned char start_code2;
	unsigned short device_id;
	int parameter;
	unsigned short command_code;
	unsigned short checksum;
};

struct DataPacket {
	unsigned char start_code1;
	unsigned char start_code2;
	unsigned short device_id;
	unsigned char* data;
	unsigned short checksum;
};

struct DeviceInfoPacket {
	unsigned long firmware_version;
	unsigned long iso_area_max_size;
	unsigned char serial_number[16];
};

template<typename T>
int calc_packet_checksum(T* packet)
{
	unsigned short checksum = 0;
	unsigned char* buffer = reinterpret_cast<unsigned char*>(packet);

	for (int i = 0; i < sizeof(T) - PACKET_CHECKSUM_SIZE; ++i)
		checksum += buffer[i];

	return checksum;
}

template<typename T>
unsigned char* create_packet(Command command, int flags)
{
	auto packet = new T();

	packet->start_code1 = START_CODE1;
	packet->start_code2 = START_CODE2;
	packet->device_id = DEVICE_ID;
	packet->parameter = flags;
	packet->command_code = command;
	packet->checksum = calc_packet_checksum<T>(packet);

	return reinterpret_cast<unsigned char*>(packet);
}

template <typename T>
T swap_endian(T u)
{
	static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

	union {
		T u;
		unsigned char u8[sizeof(T)];
	} source, dest;

	source.u = u;

	for (size_t k = 0; k < sizeof(T); k++)
		dest.u8[k] = source.u8[sizeof(T) - k - 1];

	return dest.u;
}
