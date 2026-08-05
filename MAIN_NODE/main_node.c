//main_node.c

#include <LPC21xx.h>
#include "major_type.h"
#include "major_lcd.h"
#include "major_lcd_defines.h"
#include "delay_major.h"
#include "ds18b20_major.h"
#include "can_major.h"
#include "can_defines_major.h"
#include "buzzer_major.h"

// Reverse mode flag
volatile u8 mode = 0;

// Left switch flag
volatile u8 left_flag = 0;

// Right switch flag
volatile u8 right_flag = 0;

// CAN transmit flag
volatile u8 tx_flag = 0;

// Reverse alert flag
volatile u8 reverse_alert = 0;

// Indicator command
volatile u8 indicator_cmd = 'S';

// Blink flag
u8 blink = 0;

// Temperature variables
f32 temp;
s32 tp;
u8 tpd;

// Distance value
u32 dist = 0;

// CAN timeout counter
u32 can_miss = 0;

// LCD custom characters
u8 indicator_lut[] =
{
    0x01,
    0x03,
    0x07,
    0x0F,
    0x1F,
    0x0F,
    0x07,
    0x03,

    0x10,
    0x18,
    0x1C,
    0x1E,
    0x1F,
    0x1E,
    0x1C,
    0x18
};

// Interrupt initialization
void interrupt_enable(void);

int main()
{
    // CAN frames
    struct CAN_Frame tx, rx;

    // Initialize LCD
    InitLCD();

    // Initialize CAN
    Init_CAN1();

    // Enable interrupts
    interrupt_enable();

    // Initialize buzzer
    Buzzer_init();

    // Load custom LCD symbols
    BuildCGRAM(indicator_lut,15);

    // Welcome message
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("Vehicle Safety And");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Monitoring System ");

    delay_ms(2000);

    // Clear LCD
    CmdLCD(0x01);

    while(1)
    {
        /******** LINE1 : TEMPERATURE ********/

        CmdLCD(0x80);
        StrLCD("               ");
        CmdLCD(0x80);

        // Read temperature
        temp = ReadTemp();

        // Check sensor
        if(temp == -1)
        {
            StrLCD("sensor fail");
        }
        else
        {
            // Extract integer value
            tp = ((int)temp)>>4;

            // Extract decimal value
            tpd = (((int)temp)&0x08)?'5':'0';

            // Display temperature
            StrLCD("ENG TEMP:");
            U32LCD(tp);
            CharLCD('.');
            CharLCD(tpd);
            CharLCD(223);
            CharLCD('C');
            StrLCD(" ");
        }

        /******** LINE2 : INDICATOR ********/

        // Blink effect
        blink = !blink;

        CmdLCD(0xC0);
        StrLCD("             ");
        CmdLCD(0xC0);

        // Left indicator
        if(indicator_cmd == 'L')
        {
            if(blink)
                CharLCD(0);
            else
                CharLCD(' ');

            CharLCD(1);
            StrLCD(" LEFT IND");
        }

        // Right indicator
        else if(indicator_cmd == 'R')
        {
            CharLCD(0);

            if(blink)
                CharLCD(1);
            else
                CharLCD(' ');

            StrLCD(" RIGHT IND");
        }

        // Indicator OFF
        else
        {
            CharLCD(0);
            CharLCD(1);
            StrLCD(" IND OFF");
        }

        /******** CAN TRANSMIT ********/

        // Send indicator command
        if(tx_flag == 1)
        {
            tx.ID = 0x101;
            tx.vbf.RTR = 0;
            tx.vbf.DLC = 1;
            tx.Data1 = indicator_cmd;

            CAN1_Tx(tx);

            tx_flag = 0;
        }

        // Send mode
        else if(tx_flag == 2)
        {
            tx.ID = 0x102;
            tx.vbf.RTR = 0;
            tx.vbf.DLC = 1;
            tx.Data1 = mode;

            CAN1_Tx(tx);

            tx_flag = 0;
        }

        /******** RECEIVE CAN DATA ********/

        // Check received message
        if(C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&rx);

            // Receive distance
            if(rx.ID == 0x201)
            {
                dist = rx.Data1;

                reverse_alert = rx.Data2;

                can_miss = 0;
            }
        }
        else
        {
            // CAN timeout
            can_miss++;

            if(can_miss >= 5)
            {
                dist = 999;
            }
        }

        /******** LINE3 : MODE ********/

        CmdLCD(0x94);

        // Display mode
        if(mode)
        {
            StrLCD("MODE: REVERSE ");
        }
        else
        {
            StrLCD("MODE: FORWARD ");
        }

        /******** LINE4 : DISTANCE ********/

        CmdLCD(0xD4);

        if(mode)
        {
            StrLCD("DIST:");

            // Sensor disconnected
            if(dist == 999)
            {
                StrLCD("Disconnect ");
                Buzzer_OFF();
            }
            else
            {
                // Display distance
                U32LCD(dist);

                StrLCD("cm ");

                // Obstacle detected
                if(dist < 20)
                {
                    StrLCD("ALERT   ");
                    Buzzer_ON();
                }
                else
                {
                    StrLCD("SAFE   ");
                    Buzzer_OFF();
                }
            }
        }
        else
        {
            // Forward mode display
            StrLCD("FORWARD DRIVE MODE");

            Buzzer_OFF();
        }

        // Refresh delay
        delay_ms(300);
    }
}