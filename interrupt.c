//interrupt.c

#include <LPC21xx.h>

#include "major_types.h"

extern volatile u8 mode;

extern volatile u8 left_flag;

extern volatile u8 right_flag;

extern volatile u8 tx_flag;

extern volatile u8 indicator_cmd;

void eint0_isr(void)__irq;
void eint1_isr(void)__irq;
void eint2_isr(void)__irq;

void interrupt_enable(void)
{
    PINSEL0 &= ~(
                    (3<<2) |
                    (3<<6) |
                    (3<<14)
                 );

    PINSEL0 |= (
                    (3<<2) |
                    (3<<6) |
                    (3<<14)
                );

    EXTINT=0x07;

    EXTMODE=0x07;

    EXTPOLAR=0x00;

    VICIntSelect=0x00;

    VICIntEnable=
                    (1<<14) |
                    (1<<15) |
                    (1<<16);

    VICVectCntl0=(1<<5)|14;
    VICVectAddr0=(u32)eint0_isr;

    VICVectCntl1=(1<<5)|15;
    VICVectAddr1=(u32)eint1_isr;

    VICVectCntl2=(1<<5)|16;
    VICVectAddr2=(u32)eint2_isr;
}

/****************************************
            MODE SWITCH
****************************************/

void eint0_isr(void)__irq
{
    mode=!mode;

    tx_flag=2;

    EXTINT=1<<0;

    VICVectAddr=0;
}

/****************************************
            LEFT SWITCH
****************************************/

void eint1_isr(void)__irq
{
    if(left_flag==0)
    {
        left_flag=1;

        right_flag=0;

        indicator_cmd='L';
    }
    else
    {
        left_flag=0;

        indicator_cmd='S';
    }

    tx_flag=1;

    EXTINT=1<<1;

    VICVectAddr=0;
}

/****************************************
            RIGHT SWITCH
****************************************/

void eint2_isr(void)__irq
{
    if(right_flag==0)
    {
        right_flag=1;

        left_flag=0;

        indicator_cmd='R';
    }
    else
    {
        right_flag=0;

        indicator_cmd='S';
    }

    tx_flag=1;

    EXTINT=1<<2;

    VICVectAddr=0;
}
