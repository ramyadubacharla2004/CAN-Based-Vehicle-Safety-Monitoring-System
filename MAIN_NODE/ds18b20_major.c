// ds18b20_major.c

#include <LPC21xx.h>
#include "delay_major.h"
#include "major_type.h"
#define D (1<<19)              // Data pin
#define R (IOPIN0&(1<<19))     // Read data pin

// Reset DS18B20
unsigned char ResetDS18b20(void)
{
    unsigned int presence;

    // Configure as output
    IODIR0 |= D;

    // Send reset pulse
    IOPIN0 |= D;
    delay_us(1);

    IOPIN0 &= ~(D);
    delay_us(478);

    IOPIN0 |= D;
    delay_us(54);

    // Read presence pulse
    presence = IOPIN0;

    delay_us(423);

    // Check sensor
    if(presence & R)
        return 1;
    else
        return 0;
}

// Read one bit
unsigned char ReadBit(void)
{
    unsigned int B;

    // Start read slot
    IOPIN0 &= ~(D);
    delay_us(1);

    IOPIN0 |= D;

    // Configure as input
    IODIR0 &= ~(D);

    delay_us(10);

    // Read bit
    B = IOPIN0;

    // Configure as output
    IODIR0 |= D;

    if(B & R)
        return 1;
    else
        return 0;
}

// Write one bit
void WriteBit(unsigned char Dbit)
{
    // Start write slot
    IOPIN0 &= ~(D);

    delay_us(1);

    if(Dbit)
        IOPIN0 |= D;

    delay_us(58);

    IOPIN0 |= D;

    delay_us(1);
}

// Read one byte
unsigned char ReadByte(void)
{
    unsigned char i;
    unsigned char Din = 0;

    for(i=0; i<8; i++)
    {
        Din |= ReadBit() ? (0x01<<i) : Din;

        delay_us(45);
    }

    return Din;
}

// Write one byte
void WriteByte(unsigned char Dout)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        WriteBit(Dout & 0x01);

        Dout >>= 1;

        delay_us(1);
    }

    delay_us(98);
}

// Read temperature
int ReadTemp(void)
{
    unsigned char n, buff[2];

    int temp;

    u32 timeout = 0;

    // Reset sensor
    if(ResetDS18b20())
    {
        return -1;
    }

    // Skip ROM
    WriteByte(0xCC);

    // Start conversion
    WriteByte(0x44);

    // Wait for conversion
    while(ReadByte() == 0xFF)
    {
        timeout++;

        if(timeout > 5000)
        {
            return -1;
        }
    }

    // Reset again
    if(ResetDS18b20())
    {
        return -1;
    }

    // Skip ROM
    WriteByte(0xCC);

    // Read scratchpad
    WriteByte(0xBE);

    // Read temperature bytes
    for(n=0; n<2; n++)
    {
        buff[n] = ReadByte();
    }

    // Combine MSB and LSB
    temp = buff[1];
    temp <<= 8;
    temp |= buff[0];

    return temp;
}