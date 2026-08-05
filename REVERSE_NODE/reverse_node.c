//reverse_node.c

#include <LPC21xx.h>
#include "major_type.h"
#include "delay_major.h"
#include "can_major.h"
#include "can_defines_major.h"

// Reverse mode flag
u8 mode = 0;

// Distance value
u32 distance;

// Alert flag
u8 alert;

// Function prototypes
u32 dist_cal(void);
void INIT_Ultrasonic(void);

int main()
{
    // CAN frames
    struct CAN_Frame tx, rx;

    // Initialize CAN
    Init_CAN1();

    // Initialize Ultrasonic Sensor
    INIT_Ultrasonic();

    while(1)
    {
        // Receive CAN message
        if(C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&rx);

            // Check reverse mode command
            if(rx.ID == 0x102)
            {
                mode = rx.Data1;
            }
        }

        // Reverse mode
        if(mode)
        {
            // Average of 3 readings
            distance =
            (
                dist_cal() +
                dist_cal() +
                dist_cal()
            ) / 3;

            // Check obstacle distance
            if(distance < 20)
            {
                alert = 1;
            }
            else
            {
                alert = 0;
            }

            // Prepare CAN frame
            tx.ID = 0x201;
            tx.vbf.RTR = 0;
            tx.vbf.DLC = 2;
            tx.Data1 = distance;
            tx.Data2 = alert;

            // Send CAN message
            CAN1_Tx(tx);
        }

        // Delay
        delay_ms(100);
    }
}