// indicator_node.c

#include <LPC21xx.h>
#include "major_type.h"
#include "delay_major.h"
#include "can_major.h"
#include "can_defines_major.h"

// Indicator command
u8 indicator_cmd = 'S';

// Loop variable
s32 i;

int main()
{
    // CAN receive frame
    struct CAN_Frame rx;

    // Initialize CAN
    Init_CAN1();

    // Set P0.0–P0.7 as output
    IO0DIR |= 0xFF;

    // Turn OFF all LEDs
    IO0SET = 0xFF;

    while(1)
    {
        // Receive CAN message
        if(C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&rx);

            // Read indicator command
            if(rx.ID == 0x101)
            {
                indicator_cmd = rx.Data1;
            }
        }

        // Left indicator
        if(indicator_cmd == 'L')
        {
            for(i = 0; i <= 7; i++)
            {
                IO0SET = 0xFF;

                IO0CLR = (1 << i);

                delay_ms(70);
            }
        }

        // Right indicator
        else if(indicator_cmd == 'R')
        {
            for(i = 7; i >= 0; i--)
            {
                IO0SET = 0xFF;

                IO0CLR = (1 << i);

                delay_ms(70);
            }
        }

        // Stop indicator
        else
        {
            IO0SET = 0xFF;
        }
    }
}