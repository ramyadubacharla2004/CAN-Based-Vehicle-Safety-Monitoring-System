// interrupt.c

#include <LPC21xx.h>
#include "major_type.h"

// Global variables
extern volatile u8 mode;
extern volatile u8 left_flag;
extern volatile u8 right_flag;
extern volatile u8 tx_flag;
extern volatile u8 indicator_cmd;

// ISR declarations
void eint0_isr(void)__irq;
void eint1_isr(void)__irq;
void eint2_isr(void)__irq;

// Interrupt initialization
void interrupt_enable(void)
{
    // Configure EINT0, EINT1, EINT2 pins
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

    // Clear interrupt flags
    EXTINT = 0x07;

    // Edge-triggered mode
    EXTMODE = 0x07;

    // Falling edge trigger
    EXTPOLAR = 0x00;

    // IRQ mode
    VICIntSelect = 0x00;

    // Enable interrupts
    VICIntEnable =
                    (1<<14) |
                    (1<<15) |
                    (1<<16);

    // EINT0 vector
    VICVectCntl0 = (1<<5)|14;
    VICVectAddr0 = (u32)eint0_isr;

    // EINT1 vector
    VICVectCntl1 = (1<<5)|15;
    VICVectAddr1 = (u32)eint1_isr;

    // EINT2 vector
    VICVectCntl2 = (1<<5)|16;
    VICVectAddr2 = (u32)eint2_isr;
}

/******** MODE SWITCH ********/

void eint0_isr(void)__irq
{
    // Toggle mode
    mode = !mode;

    // Send mode
    tx_flag = 2;

    // Clear interrupt
    EXTINT = 1<<0;

    // End ISR
    VICVectAddr = 0;
}

/******** LEFT SWITCH ********/

void eint1_isr(void)__irq
{
    // Toggle left indicator
    if(left_flag == 0)
    {
        left_flag = 1;
        right_flag = 0;
        indicator_cmd = 'L';
    }
    else
    {
        left_flag = 0;
        indicator_cmd = 'S';
    }

    // Send command
    tx_flag = 1;

    // Clear interrupt
    EXTINT = 1<<1;

    // End ISR
    VICVectAddr = 0;
}

/******** RIGHT SWITCH ********/

void eint2_isr(void)__irq
{
    // Toggle right indicator
    if(right_flag == 0)
    {
        right_flag = 1;
        left_flag = 0;
        indicator_cmd = 'R';
    }
    else
    {
        right_flag = 0;
        indicator_cmd = 'S';
    }

    // Send command
    tx_flag = 1;

    // Clear interrupt
    EXTINT = 1<<2;

    // End ISR
    VICVectAddr = 0;
}