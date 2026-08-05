//ultrasonic.c

#include <LPC21xx.h>
#include "delay_major.h"
#include "major_type.h"

#define trr_pin 20      // Trigger pin
#define echo_pin 19     // Echo pin

// Ultrasonic initialization
void INIT_Ultrasonic(void)
{
    // Trigger as output
    IODIR0 |= (1<<trr_pin);

    // Echo as input
    IODIR0 &= ~(1<<echo_pin);

    // Timer0 initialization
    T0PR = 15;      // 1 µs timer

    T0TCR = 0x00;
}

// Send trigger pulse
void send_pulse(void)
{
    IOCLR0 = (1<<trr_pin);

    delay_us(2);

    IOSET0 = (1<<trr_pin);

    delay_us(10);

    IOCLR0 = (1<<trr_pin);
}

// Read echo pulse
unsigned int read_echo(void)
{
    u32 timeout = 0;

    // Wait for echo HIGH
    while(((IOPIN0 >> echo_pin) & 1) == 0)
    {
        timeout++;

        // Timeout check
        if(timeout > 30000)
        {
            return 0;
        }
    }

    // Start timer
    T0TCR = 0x02;
    T0TCR = 0x01;

    timeout = 0;

    // Wait for echo LOW
    while((IOPIN0 >> echo_pin) & 1)
    {
        timeout++;

        // Timeout check
        if(timeout > 30000)
        {
            T0TCR = 0x00;
            return 0;
        }
    }

    // Stop timer
    T0TCR = 0x00;

    return T0TC;
}

// Calculate distance
unsigned int dist_cal(void)
{
    u32 pulse;
    u32 distance;

    // Send trigger
    send_pulse();

    // Read pulse width
    pulse = read_echo();

    // Sensor failure
    if(pulse == 0)
    {
        return 999;
    }

    // Convert to cm
    distance = pulse / 58;

    // Limit maximum distance
    if(distance > 400)
    {
        distance = 400;
    }

    return distance;
}