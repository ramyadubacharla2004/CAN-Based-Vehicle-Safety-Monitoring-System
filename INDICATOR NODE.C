INDICATOR_NODE

//can_major.c
#include <lpc21xx.h>
#include "major_types.h"
#include"can_major.h"
#include"can_defines_major.h"
/*CAN Controller 1 Initialization : (defined in can.c )*/
void Init_CAN1(void)
{
    //cfg p0.25 pin as CAN1_RX pin(RD1),TD1 is exclusive    
    PINSEL1|=RD1_PIN; //using defines from can_defines.h   
                      // #define RD1_PIN 0x00040000 ,    
                      //as RD1/ (i.e CAN1_RX)/p0.25   
    //Reset CAN1 controller   
    C1MOD=1;
    //All received messages are accepted       
    AFMR=2;
    //Set baud Rate for CAN   
    C1BTR=BTR_LVAL; //using defines from can_defines.h   
    //Enable CAN1 controller   
    C1MOD=0;
}
void CAN1_Tx(struct CAN_Frame txFrame)
{
  // Checking that the TX buffer is empty in C1GSR     
     while((C1GSR&TBS1_BIT_READ)==0); //if status is 1 then empty    

  // place 11-bit tx id in C1T1D1     
                C1TID1=txFrame.ID;
  // place cfg whether data/remote frame & no of data bytes in message      
    C1TFI1=txFrame.vbf.RTR<<30|txFrame.vbf.DLC<<16;
  // For Data Frame place 1 to 8 bytes data into Data Tx Buffers  
        if(txFrame.vbf.RTR!=1)
    {
          //Place data bytes 1-4 in C1TDA1      

                        C1TDA1= txFrame.Data1;
        //Place data bytes 5-8 in C1TDB1     
      C1TDB1= txFrame.Data2;
                }
    //Select Tx Buf1 & Start Xmission using       
     C1CMR=STB1_BIT_SET|TR_BIT_SET;
                //monitor tx status in C1GSR     
      while((C1GSR&TCS1_BIT_READ)==0);
}
void CAN1_Rx(struct CAN_Frame *rxFrame)
{
//wait for CAN frame recv status        
        while((C1GSR&RBS_BIT_READ)==0);
//read 11-bit CANid of recvd frame.         
        rxFrame->ID=C1RID;
// read & extract data/remote frame status        
        rxFrame->vbf.RTR=(C1RFS>>30)&1;
//read & extract data length        
        rxFrame->vbf.DLC=(C1RFS>>16)&0x0f;
//check if recvd frame is data frame,extract data bytes        
        if(rxFrame->vbf.RTR==0)
  {
//read 1-4 bytes from C1RDA        
                rxFrame->Data1=C1RDA;
//read 5-8 bytes from C1RDB        
    rxFrame->Data2=C1RDB;
 }

// Release receive buffer         

      C1CMR=RRB_BIT_SET;

}

//can_major.h
#ifndef __CAN_H__
#define __CAN_H__
#include "major_types.h"
struct CAN_Frame
{
        u32 ID;
        struct BitField
        {
                u8 RTR : 1;
                u8 DLC : 4;
        }vbf;
        u32 Data1,Data2;//8-bytes
};
void Init_CAN1(void);
void CAN1_Tx(struct CAN_Frame);
void CAN1_Rx(struct CAN_Frame *);
#endif


//can_defines_major.h
//defines required for C1BTR in can_defines.h     
#define RD1_PIN 0x00040000 


#define PCLK       60000000  //Hz    
#define BIT_RATE   125000    //Hz    
#define QUANTA     16    
#define BRP        (PCLK/(BIT_RATE*QUANTA))    
#define SAMPLE_POINT  (0.7 * QUANTA)    
#define TSEG1        ((int)SAMPLE_POINT-1)  //TSEG1=prop_seg+Tph1_seg  
#define TSEG2        (QUANTA-(1+TSEG1))   //TSEG2=Tph2_seg  
#define SJW          ((TSEG2 >= 5)   ?   4 : (TSEG2-1))     
#define SAM          0 //0 or 1 ,sample bus 1 or 3 time(s)    
#define BTR_LVAL    (SAM<<23|(TSEG2-1)<<20|(TSEG1-1)<<16|(SJW-1)<<14|(BRP-1))    
//defines for C1CMR bit set     
#define TR_BIT_SET   1<<0     
#define RRB_BIT_SET  1<<2      
#define STB1_BIT_SET 1<<5         

//defines for C1GSR bit check     
#define RBS_BIT_READ  1<<0     
#define TBS1_BIT_READ 1<<2     
#define TCS1_BIT_READ 1<<3     

//defines for C1CMR bit set
#define TR_BIT_SET   1<<0
#define RRB_BIT_SET  1<<2 
#define STB1_BIT_SET 1<<5
//defines for C1GSR bit check
#define RBS_BIT_READ  1<<0
#define TBS1_BIT_READ 1<<2
#define TCS1_BIT_READ 1<<3

//major_type.h
typedef unsigned char u8;
typedef char s8;
typedef unsigned int u32;
typedef int s32;       
typedef unsigned short int u16;
typedef signed short int s16;
typedef float f32;
typedef double d64;

//delay_major.c
#include "delay_major.h"
#include"major_types.h"

void delay_MS(unsigned int dly)
{
        unsigned int i;
        for(;dly>0;dly--)
        {
                for(i=0;i<1200;i++);
        }
}
void delay_us(u32 dlyus)
{
   for(dlyus*=12;dlyus>0;dlyus--);
}
void delay_ms(u32 dlyms)
{
   for(dlyms*=12000;dlyms>0;dlyms--);
}
void delay_s(u32 dlys)
{
   for(dlys*=12000000;dlys>0;dlys--);
}

//delay_major.h
void delay_MS(unsigned int dly);
void delay_us(unsigned int dlyus);
void delay_ms(unsigned  int dlyms);
void delay_s(unsigned int dlys);

//indicator_node.c

#include <LPC21xx.h>

#include "major_types.h"
#include "delay_major.h"
#include "can_major.h"
#include "can_defines_major.h"

u8 indicator_cmd='S';

s32 i;

int main()
{
    struct CAN_Frame rx;

    Init_CAN1();

    /******** P0.0 TO P0.7 OUTPUT ********/

    IO0DIR |= 0xFF;

    /******** ACTIVE LOW LEDS OFF ********/

    IO0SET = 0xFF;

    while(1)
    {
        /****************************************
                RECEIVE CAN DATA
        ****************************************/

        if(C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&rx);

            if(rx.ID==0x101)
            {
                indicator_cmd=rx.Data1;
            }
        }

        /****************************************
                LEFT INDICATOR
        ****************************************/

        if(indicator_cmd=='L')
        {
            for(i=0;i<=7;i++)
            {
                IO0SET=0xFF;

                IO0CLR=(1<<i);

                delay_ms(70);
            }
        }

        /****************************************
                RIGHT INDICATOR
        ****************************************/

        else if(indicator_cmd=='R')
        {
            for(i=7;i>=0;i--)
            {
                IO0SET=0xFF;

                IO0CLR=(1<<i);

                delay_ms(70);
            }
        }

        /****************************************
                    STOP
        ****************************************/

        else
        {
            IO0SET=0xFF;
        }
    }
}




