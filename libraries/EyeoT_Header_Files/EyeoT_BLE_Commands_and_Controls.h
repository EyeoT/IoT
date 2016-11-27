#include <CurieBLE.h>         // 101-specific BLE code

// BLE commands (what the Arduino slave receives (Rx) from the master Linux PC)
const unsigned short NO_LIGHT = 0;
const unsigned short RED_LIGHT = 1;
const unsigned short GREEN_LIGHT = 2;
const unsigned short BLUE_LIGHT = 3;
const unsigned short SERVO_ON = 4;
const unsigned short SERVO_OFF = 5;

// BLE responses (what the Arduino slave writes to itself and is read by the master Linux PC)
const unsigned short NOT_READY = 6;
const unsigned short READY = 7;
const unsigned short COMMAND_ACK = 8;
const unsigned short COMMAND_SUCCESS = 9;
const unsigned short COMMAND_FAILURE = 10;

BLEPeripheral blePeripheral; // create peripheral instance
BLEService eyeoTService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service
// create switch characteristic and allow remote device to read and write
BLEUnsignedShortCharacteristic currentCommand("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEUnsignedShortCharacteristic currentResponse("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead);




