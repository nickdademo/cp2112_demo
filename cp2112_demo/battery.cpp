#include "battery.h"

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
    BYTE                numBytesRead = 0;

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
    }
    else
    {
        return -1;
    }

    // Success
    return 0;
}
