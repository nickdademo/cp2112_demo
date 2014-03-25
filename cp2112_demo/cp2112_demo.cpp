#include <windows.h>
#include <stdio.h>
#include "battery.h"

#define BITRATE_HZ          50000
#define ACK_ADDRESS         0x02
#define AUTO_RESPOND        FALSE
#define WRITE_TIMEOUT_MS    1000
#define READ_TIMEOUT_MS     1000
#define TRANSFER_RETRIES    3
#define SCL_LOW_TIMEOUT     FALSE
#define RESPONSE_TIMEOUT_MS 1000

int main(int argc, char* argv[])
{
    HID_SMBUS_DEVICE    m_hidSmbus;
    BYTE                buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE];

    // Open device
    if(SMBus_Open(&m_hidSmbus) != 0)
    {
        fprintf(stderr,"ERROR: Could not open device.\r\n");
        SMBus_Close(&m_hidSmbus);
        return -1;
    }
    else
    {
        fprintf(stderr,"Device successfully opened.\r\n");
    }

    // Configure device
    if(SMBus_Configure(&m_hidSmbus, BITRATE_HZ, ACK_ADDRESS, AUTO_RESPOND, WRITE_TIMEOUT_MS, READ_TIMEOUT_MS, SCL_LOW_TIMEOUT, TRANSFER_RETRIES, RESPONSE_TIMEOUT_MS) != 0)
    {
        fprintf(stderr,"ERROR: Could not configure device.\r\n");
        SMBus_Close(&m_hidSmbus);
        return -1;
    }
    else
    {
        fprintf(stderr,"Device successfully configured.\r\n");
    }
    
    // Read Pack Voltage [0x09]
    if(SMBus_Read(&m_hidSmbus, buffer, 0x16, 0x09, 2) != 0)
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(&m_hidSmbus);
        return -1;
    }
    else
    {
        unsigned int packVoltage_mV = (buffer[1] << 8) | buffer[0];
        fprintf(stderr, "Pack Voltage = %d mV\r\n", packVoltage_mV);
    }

    // Success
    SMBus_Close(&m_hidSmbus);
    return 0;
}