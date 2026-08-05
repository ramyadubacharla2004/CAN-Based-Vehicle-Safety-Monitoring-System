//can_major.c

#include <lpc21xx.h>
#include "major_type.h"
#include "can_major.h"
#include "can_defines_major.h"

// CAN1 Initialization
void Init_CAN1(void)
{
    // Configure CAN RX pin
    PINSEL1 |= RD1_PIN;

    // Reset CAN
    C1MOD = 1;

    // Accept all messages
    AFMR = 2;

    // Set baud rate
    C1BTR = BTR_LVAL;

    // Enable CAN
    C1MOD = 0;
}

// CAN Transmit Function
void CAN1_Tx(struct CAN_Frame txFrame)
{
    // Wait for empty TX buffer
    while((C1GSR & TBS1_BIT_READ) == 0);

    // Load CAN ID
    C1TID1 = txFrame.ID;

    // Load RTR and DLC
    C1TFI1 = txFrame.vbf.RTR << 30 | txFrame.vbf.DLC << 16;

    // Load data for Data Frame
    if(txFrame.vbf.RTR != 1)
    {
        C1TDA1 = txFrame.Data1;
        C1TDB1 = txFrame.Data2;
    }

    // Start transmission
    C1CMR = STB1_BIT_SET | TR_BIT_SET;

    // Wait until transmission completes
    while((C1GSR & TCS1_BIT_READ) == 0);
}

// CAN Receive Function
void CAN1_Rx(struct CAN_Frame *rxFrame)
{
    // Wait for received message
    while((C1GSR & RBS_BIT_READ) == 0);

    // Read CAN ID
    rxFrame->ID = C1RID;

    // Read RTR bit
    rxFrame->vbf.RTR = (C1RFS >> 30) & 1;

    // Read DLC
    rxFrame->vbf.DLC = (C1RFS >> 16) & 0x0F;

    // Read data
    if(rxFrame->vbf.RTR == 0)
    {
        rxFrame->Data1 = C1RDA;
        rxFrame->Data2 = C1RDB;
    }

    // Release receive buffer
    C1CMR = RRB_BIT_SET;
}