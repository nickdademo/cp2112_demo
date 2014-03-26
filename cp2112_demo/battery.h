#include <windows.h>
#include "SLABCP2112.h"
#pragma comment(lib, "lib/x86/SLABHIDtoSMBus.lib")

#define VID                     0x10C4
#define PID                     0xEA90
#define SLAVE_WRITE_ADDRESS     0x16
#define TARGET_ADDRESS_SIZE     1

enum sbsCommands {
    MANUFACTURER_ACCESS, // 0x00
    REMAINING_CAPACITY_ALARM,
    REMAINING_TIME_ALARM,
    BATTERY_MODE,
    AT_RATE,
    AT_RATE_TIME_TO_FULL,
    AT_RATE_TIME_TO_EMPTY,
    AT_RATE_OK,
    TEMPERATURE,
    VOLTAGE,
    CURRENT,
    AVERAGE_CURRENT,
    MAX_ERROR,
    RELATIVE_STATE_OF_CHARGE,
    ABSOLUTE_STATE_OF_CHARGE,
    REMAINING_CAPACITY,
    FULL_CHARGE_CAPACITY,
    RUN_TIME_TO_EMPTY,
    AVERAGE_TIME_TO_EMPTY,
    AVERAGE_TIME_TO_FULL,
    CHARGING_CURRENT,
    CHARGING_VOLTAGE,
    BATTERY_STATUS,
    CYCLE_COUNT,
    DESIGN_CAPACITY,
    DESIGN_VOLTAGE,
    SPECIFICATION_INFO,
    MANUFACTURER_DATE,
    SERIAL_NUMBER,
    RESERVED1,
    RESERVER2,
    RESERVED3,
    MANUFACTURER_NAME,
    DEVICE_NAME,
    DEVICE_CHEMISTRY,
    MANUFACTURER_DATA // 0x23
};

extern const int sbsCommandResponseLength[];

int SMBus_Open(HID_SMBUS_DEVICE* device);
int SMBus_Close(HID_SMBUS_DEVICE* device);
int SMBus_Reset(HID_SMBUS_DEVICE* device);
int SMBus_Configure(HID_SMBUS_DEVICE* device, DWORD bitRate, BYTE address, BOOL autoReadRespond, WORD writeTimeout, WORD readTimeout, BOOL sclLowTimeout, WORD transferRetries, DWORD responseTimeout);
int SMBus_Read(HID_SMBUS_DEVICE* device, BYTE* buffer, BYTE slaveAddress, BYTE targetAddress, WORD numBytesToRead);