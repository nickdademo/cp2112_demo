#include "battery.h"

/*
NOTE: As command response lengths may differ between gas gauges, ensure
      sbsCommandResponseLength contains the correct lengths for your
      particular device (check datasheet).

For example, some typical variations:
    Manufacturer Name [0x20] = 20+1 bytes / 11+1 bytes
    Device Name [0x21] = 20+1 bytes / 7+1 bytes
*/
const int sbsCommandResponseLength[] = {
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   // 0x00 - 0x09
    2, 2, 1, 1, 1, 2, 2, 2, 2, 2,   // 0x0A - 0x13
    2, 2, 2, 2, 2, 2, 2, 2, 2, 0,   // 0x14 - 0x1D
    0, 0, 21, 21, 5, 15             // 0x1E - 0x23
};

int SMBus_Open(HID_SMBUS_DEVICE* device)
{
    BOOL                    found = FALSE;
    DWORD                   deviceNum;
    DWORD                   numDevices;
    HID_SMBUS_DEVICE_STR    deviceString;
    HID_SMBUS_STATUS        status;

    // Search for device
    if(HidSmbus_GetNumDevices(&numDevices, VID, PID) == HID_SMBUS_SUCCESS)
    {
        for (DWORD i = 0; i < numDevices; i++)
        {
            if(HidSmbus_GetString(i, VID, PID, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
            {
                deviceNum = i;
                found = TRUE;
                break;
            }
        }
    }

    // Device not found
    if(!found)
    {
        return -1;
    }
    // Device found
    else
    {
        // Attempt open
        status = HidSmbus_Open(device, deviceNum, VID, PID);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return -1;
        }
    }

    // Success
    return 0;
}

int SMBus_Close(HID_SMBUS_DEVICE* device)
{
    HID_SMBUS_STATUS status;

    // Attempt close
    status = HidSmbus_Close(device);
    // Check status
    if(status != HID_SMBUS_SUCCESS)
    {
        return -1;
    }

    return 0;
}

int SMBus_Reset(HID_SMBUS_DEVICE* device)
{
    BOOL                opened;
    HID_SMBUS_STATUS    status;

    // Make sure that the device is opened
    if(HidSmbus_IsOpened(*device, &opened) == HID_SMBUS_SUCCESS && opened)
    {
        // Attempt reset
        status = HidSmbus_Reset(device);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return -1;
        }
    }

    return 0;
}

int SMBus_Configure(HID_SMBUS_DEVICE* device, DWORD bitRate, BYTE address, BOOL autoReadRespond, WORD writeTimeout, WORD readTimeout, BOOL sclLowTimeout, WORD transferRetries, DWORD responseTimeout)
{
    BOOL                opened;
    HID_SMBUS_STATUS    status;

    // Make sure that the device is opened
    if(HidSmbus_IsOpened(*device, &opened) == HID_SMBUS_SUCCESS && opened)
    {
        // Attempt configuration
        status =  HidSmbus_SetSmbusConfig(*device, bitRate, address, autoReadRespond, writeTimeout, readTimeout, sclLowTimeout, transferRetries);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return -1;
        }

        // Set response timeout
        status = HidSmbus_SetTimeouts(*device, responseTimeout);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return -1;
        }
    }

    return 0;
}

int SMBus_Read(HID_SMBUS_DEVICE* device, BYTE* buffer, BYTE slaveAddress, BYTE targetAddress, WORD numBytesToRead)
{
    BOOL                opened;
    HID_SMBUS_STATUS    status;
    HID_SMBUS_S0        status0;
    HID_SMBUS_S1        status1;
    BYTE                numBytesRead = 0;
    WORD                numRetries;
    WORD                bytesRead;

    // Make sure that the device is opened
    if(HidSmbus_IsOpened(*device, &opened) == HID_SMBUS_SUCCESS && opened)
    {
        // Issue an address read request
        status = HidSmbus_AddressReadRequest(*device, slaveAddress, numBytesToRead, TARGET_ADDRESS_SIZE, &targetAddress);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return -1;
        }

        // Issue transfer status request
        status = HidSmbus_TransferStatusRequest(*device);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return -1;
        }

        // Wait for transfer status response
        status = HidSmbus_GetTransferStatusResponse(*device, &status0, &status1, &numRetries, &bytesRead);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return -1;
        }

        // Notify device that it should send a read response back
        status = HidSmbus_ForceReadResponse(*device, numBytesToRead);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return -1;
        }

        // Wait for a read response
        status = HidSmbus_GetReadResponse(*device, &status0, buffer, HID_SMBUS_MAX_READ_RESPONSE_SIZE, &numBytesRead);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return -1;
        }
        // Check number of bytes read
        if(numBytesRead != numBytesToRead)
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    // Success
    return 0;
}
