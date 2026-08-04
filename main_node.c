//main_node.c

#include <LPC21xx.h>

#include "major_types.h"
#include "major_lcd.h"
#include "major_lcd_defines.h"
#include "delay_major.h"
#include "ds18b20_major.h"
#include "can_major.h"
#include "can_defines_major.h"
#include "buzzer_major.h"

volatile u8 mode=0;

volatile u8 left_flag=0;

volatile u8 right_flag=0;

volatile u8 tx_flag=0;

volatile u8 reverse_alert=0;

volatile u8 indicator_cmd='S';

u8 blink=0;

f32 temp;

s32 tp;

u8 tpd;

u32 dist=0;
 
u32 can_miss=0;

u8 indicator_lut[]=
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

void interrupt_enable(void);

int main()
{
    struct CAN_Frame tx,rx;

    InitLCD();

    Init_CAN1();

    interrupt_enable();

    Buzzer_init();

    BuildCGRAM(indicator_lut,15);

    /******** SYSTEM DISPLAY ********/

    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("Vehicle Safety And");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Monitoring System ");

    delay_ms(2000);
	CmdLCD(0x01);

    while(1)
    {
        /****************************************
                LINE1 : TEMPERATURE
        ****************************************/
	   CmdLCD(0x80);
	   StrLCD("               ");
	   CmdLCD(0x80);
        temp=ReadTemp();

        //CmdLCD(0x80);

        if(temp==-1)
        {
            StrLCD("sensor fail");
        }
        else
        {
            tp=((int)temp)>>4;

            tpd=(((int)temp)&0x08)?'5':'0';

            StrLCD("ENG TEMP:");

            U32LCD(tp);

            CharLCD('.');

            CharLCD(tpd);

            CharLCD(223);

            CharLCD('C');

            StrLCD(" ");
        }

        /****************************************
            LINE2 : INDICATOR STATUS
        ****************************************/

      /*  blink=!blink;

        CmdLCD(0xc0);

        if(indicator_cmd=='L')
        {
            if(blink)
                CharLCD(0);
            else
                CharLCD(' ');

            StrLCD(" LEFT IND     ");
        }

        else if(indicator_cmd=='R')
        {
            if(blink)
                CharLCD(1);
            else
                CharLCD(' ');

            StrLCD(" RIGHT IND    ");
        }

        else
        {
            CharLCD(0);

            CharLCD(1);

            StrLCD(" IND OFF      ");
        }*/
		 blink=!blink;

        CmdLCD(0xc0);
		StrLCD("             ");
		CmdLCD(0xc0);

        if(indicator_cmd=='L')
        {
            if(blink)
                CharLCD(0);
            else
                CharLCD(' ');
					 CharLCD(1);
            StrLCD(" LEFT IND");
        }

        else if(indicator_cmd=='R')
        {
		CharLCD(0);
            if(blink)
                CharLCD(1);
            else
                CharLCD(' ');
            StrLCD(" RIGHT IND");
        }

        else
        {
            CharLCD(0);

            CharLCD(1);

            StrLCD(" IND OFF");
        }


        /****************************************
                CAN TRANSMISSION
        ****************************************/

        if(tx_flag==1)
        {
            tx.ID=0x101;

            tx.vbf.RTR=0;

            tx.vbf.DLC=1;

            tx.Data1=indicator_cmd;

            CAN1_Tx(tx);

            tx_flag=0;
        }

        else if(tx_flag==2)
        {
            tx.ID=0x102;

            tx.vbf.RTR=0;

            tx.vbf.DLC=1;

            tx.Data1=mode;

            CAN1_Tx(tx);

            tx_flag=0;
        }

        /****************************************
                RECEIVE DISTANCE + ALERT
        ****************************************/

        if(C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&rx);

            if(rx.ID==0x201)
            {
                dist=rx.Data1;

                reverse_alert=rx.Data2;
				can_miss=0;
            }
        }
		else
		{
		can_miss++;
		if(can_miss>=5)
		{
		dist=999;
		}
		}


        /****************************************
                LINE3 : MODE DISPLAY
        ****************************************/

        CmdLCD(0x94);

        if(mode)
        {
            StrLCD("MODE: REVERSE ");
        }
        else
        {
            StrLCD("MODE: FORWARD ");
        }

        /****************************************
            LINE4 : STATUS / DISTANCE
        ****************************************/

        CmdLCD(0xd4);

        if(mode)
        {
            StrLCD("DIST:");
			if(dist==999)
			{
			StrLCD("Disconnect ");
			Buzzer_OFF();
			}
			else
			{

            U32LCD(dist);

            StrLCD("cm ");

            /*if(reverse_alert)
            {
                StrLCD("ALERT ");

                Buzzer_ON();
            }
            else
            {
                StrLCD("SAFE  ");

                Buzzer_OFF();
            } */
			if(dist<20)
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
            StrLCD("FORWARD DRIVE MODE");

            Buzzer_OFF();
        }

        delay_ms(300);
    }

}	
