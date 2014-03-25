#include <windows.h>
#include <stdio.h>
#include "SLABCP2112.h"
#pragma comment(lib, "lib/x86/SLABHIDtoSMBus.lib")

#define VID 0x10C4
#define PID 0xEA90

int SMBus_Open(HID_SMBUS_DEVICE* device);
int SMBus_Close(HID_SMBUS_DEVICE* device);
int SMBus_Reset(HID_SMBUS_DEVICE* device);
int SMBus_Configure(HID_SMBUS_DEVICE* device, DWORD bitRate, BYTE address, BOOL autoReadRespond, WORD writeTimeout, WORD readTimeout, BOOL sclLowTimeout, WORD transferRetries, DWORD responseTimeout);
int SMBus_Read(HID_SMBUS_DEVICE* device, BYTE* buffer, BYTE slaveAddress, BYTE targetAddress, WORD numBytesToRead);