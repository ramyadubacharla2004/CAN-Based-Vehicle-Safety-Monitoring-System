MAIN_NODE

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

//major_lcd.c
#include<lpc21xx.h>
#include "major_lcd_defines.h"
#include "major_types.h"
#include "defines_major.h"
#include "delay_major.h"
#include "major_lcd.h"
void WriteLCD(u8 data)
{
	SCLRBIT(IOCLR0,RW);//RW=0, write operation
	WBYTE(IOPIN0,LCD_DATA,data);
	SSETBIT(IOSET0,EN);//EN=1
	delay_us(1);
	SCLRBIT(IOCLR0,EN);//EN=0
	delay_ms(2);//internal process
}
void CmdLCD(u8 cmd)
{
	SCLRBIT(IOCLR0,RS);//RS=0, command reg selected
	WriteLCD(cmd);
}
void CharLCD(u8 ascii)
{
	SSETBIT(IOSET0,RS);//RS=1, data reg selected
	WriteLCD(ascii);
}
void InitLCD(void)
{
	WBYTE(IODIR0,LCD_DATA,255); //P0.8 to P0.15 as outputs
	SETBIT(IODIR0,RS);//p0.16 as output
	SETBIT(IODIR0,RW);//p0.17 as output
	SETBIT(IODIR0,EN);//p0.18 as output
	
	delay_ms(15);
	CmdLCD(MODE_8BIT_1LINE);
	delay_ms(5);
	CmdLCD(MODE_8BIT_1LINE);
	delay_us(100);
	CmdLCD(MODE_8BIT_1LINE);
	
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DISP_ON);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
}

void StrLCD(s8* p)
{
	while(*p)
	CharLCD(*p++);
}

void U32LCD(u32 n)
{
	u8 a[10];
	s32 i=0;
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n)
		{
			a[i++]=(n%10)+48;
			n/=10;
		}
		for(--i;i>=0;i--)
		CharLCD(a[i]);
	}
	
}

void S32LCD(s32 n)
{
	if(n<0)
	{
		CharLCD('-');
		n=-n;
		U32LCD(n);
	}
	
}
void F32LCD(f32 fnum, u8 nDP)
{
	u32 n;
	s32 i;
	if(fnum<0)
	{
		CharLCD('-');
	}
		n=fnum;
		U32LCD(n);
		CharLCD('.');
		for(i=0; i<nDP; i++)
		{
			fnum=(fnum-n)*10;
			n=fnum;
			CharLCD(n+48);
		}
	
}

void HexLCD(u32 n)
{
	u8 a[8],rem;
	s32 i=0;
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n)
		{
			rem =n%16;
			(rem<10)?(rem+=48):(rem+=55);
			a[i++]=rem;
			n/=16;
		}
		for(--i; i>=0; i--)
		{
			CharLCD(a[i]);
		}
	}
}

void BinLCD(u32 n, u8 nbd)
{
	s32 i;
	for(i=(nbd-1); i>=0; i--)
	{
		CharLCD(((n>>i)&1)+48);
	}
}
void BuildCGRAM(u8* p, u8 nb)
{
	s32 i;
	CmdLCD(GOTO_CGRAM);
	for(i=0; i<=nb; i++)
	{
		CharLCD(p[i]);
	}
	CmdLCD(GOTO_LINE2_POS0);
}

//major_lcd.h
#ifndef _LCD_H_

#define _LCD_H_


void WriteLCD(unsigned char data);


void CmdLCD(unsigned char cmd);


void CharLCD(unsigned char ascii);


void InitLCD(void);


void StrLCD(char *p);


void U32LCD(unsigned int n);


void F32LCD(float fnum,unsigned char ndp);


void S32LCD(int n);


void HexLCD(unsigned int n);


void BinLCD(unsigned int n,unsigned char nbd);


void OctLCD(unsigned int n);


void BuildCGRAM(unsigned char *p,unsigned char nb);
#endif

//major_lcd_defines.h
//lcd_defines.h
#define LCD_DATA 8
#define RW 18
#define RS 16
#define EN 17
//LCD COMMANDS
#define CLEAR_LCD 0x01
#define RET_CUR_HOME 0X02
#define DISP_OFF 0X08
#define DISP_ON 0X0C
#define DISP_ON_CUR_ON 0X0E
#define DISP_ON_CUR_ON_BLINK 0X0F
#define MODE_8BIT_1LINE 0X30
#define MODE_8BIT_2LINE 0X38
#define MODE_4BIT_1LINE 0X20
#define MODE_4BIT_2LINE 0X28
#define GOTO_LINE1_POS0 0X80
#define GOTO_LINE2_POS0 0XC0
#define GOTO_LINE3_POS0 0X94
#define GOTO_LINE4_POS0 0XD4
#define SHIFT_CUR_RIGHT 0X06
#define SHIFT_DISP_LEFT 0X10
#define SHIFT_DISP_RIGHT 0X14
#define GOTO_CGRAM 0X40

//buzzer_major.c
#include<lpc21xx.h>
#include"buzzer_major.h"
#define BUZZER 20
void Buzzer_init(void)
{
IODIR0|=(1<<BUZZER);
IOCLR0|=(1<<BUZZER);
}
void Buzzer_ON(void)
{
IOCLR0|=(1<<BUZZER);
}
void Buzzer_OFF(void)
{
IOSET0|=(1<<BUZZER);
}


//buzzer_major.h
#ifndef _BUZZEr_H_
#define	 _BUZZER_H_
void Buzzer_init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);
#endif 

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

//major_type.h
typedef unsigned char u8;
typedef char s8;
typedef unsigned int u32;
typedef int s32;       
typedef unsigned short int u16;
typedef signed short int s16;
typedef float f32;
typedef double d64;

//ds18b20_major.c
#include<LPC21xx.h>
#include "delay_major.h"
#include "major_types.h"

#define D 1<<19          //1-WIRE DATA LINE
#define R (IOPIN0&(1<<19))


/**RESET FUNCTION DEFINATION FOR DS18B20**/
unsigned char ResetDS18b20(void)
{
        unsigned int  presence;
    IODIR0 |= D;
        IOPIN0 |= D;
        delay_us(1); //////////////1Us
        IOPIN0 &= ~(D);
        delay_us(478);//////////////486Us 
        IOPIN0 |= D;
        delay_us(54); //////////////55Us
        presence = IOPIN0 ;
        delay_us(423);//////////////431Us

        if(presence&R)
                return 1;
        else
                return 0;
}

/**READING A BIT USING 1-WIRE PROTOCOL FROM DS18B20**/
unsigned char ReadBit(void)
{
 unsigned int B;
        IOPIN0 &= ~(D);
        delay_us(1);//////////////1Us
        IOPIN0 |= D;
        IODIR0 &= ~(D);
        delay_us(10);//////////////11Us
        B=IOPIN0;
        IODIR0 |=D;
        if(B&R)
                return 1;
        else
                return 0;
}
/**WRITING A BIT USING 1-WIRE PROTOCOL INTO DS18B20**/
void WriteBit(unsigned char Dbit)
{
        IOPIN0 &= ~(D);
        delay_us(1);//////////////1Us
        if(Dbit)
                IOPIN0 |= D;
        delay_us(58);//////////////59Us
        IOPIN0 |= D;
        delay_us(1);
}
/**READING A BYTE USING 1-WIRE PROTOCOL FROM DS18B20**/
unsigned char ReadByte(void)
{
        unsigned char i;
        unsigned char Din = 0;
        for (i=0;i<8;i++)
 {
                Din|=ReadBit()? 0x01<<i:Din;
                delay_us(45); //////////////46Us
        }
        return(Din);
}

/**WRITING A BIT USING 1-WIRE PROTOCOL INTO DS18B20**/
void WriteByte(unsigned char Dout)
{
        unsigned char i;
        for (i=0; i<8; i++)
        {
                WriteBit((Dout & 0x1));
                Dout = Dout >> 1;
                delay_us(1);
        }
        delay_us(98);//////////////100Us
}

/**READING TEMPERATURE FROM DS18B20 USING 1-WIRE PROTOCOL**/
/*int ReadTemp(void)
{
        unsigned char n,buff[2];
        int temp;
        ResetDS18b20();   //RESET DS18B20
    WriteByte(0xcc);   // skip ROM
    WriteByte(0x44);   // perform temperature conversion
	//delay_ms(750);//required for 12 bits resolution
    while (ReadByte()==0xff); // wait for conversion complete   
        ResetDS18b20();
        WriteByte(0xcc);   // skip ROM
        WriteByte(0xbe);    // read the result
    for (n=0; n<2; n++) // read 9 bytes but, use only one byte
    {
       buff[n]=ReadByte();  // read DS1820
    }
        temp=buff[1];
        temp=temp<<8;
        temp=temp|buff[0];
        return(temp);
} */
int ReadTemp(void)

{

    unsigned char n,buff[2];

    int temp;

    u32 timeout=0;

    /******** RESET ********/

    if(ResetDS18b20())

    {

        return -1;

    }

    WriteByte(0xcc); // Skip ROM

    WriteByte(0x44); // Start conversion

    /******** WAIT FOR CONVERSION ********/

    while(ReadByte()==0xff)

    {

        timeout++;

        if(timeout>5000)

        {

            return -1;

        }

    }

    /******** RESET AGAIN ********/

    if(ResetDS18b20())

    {

        return -1;

    }

    WriteByte(0xcc);

    WriteByte(0xbe);

    /******** READ TEMP ********/

    for(n=0;n<2;n++)

    {

        buff[n]=ReadByte();

    }

    temp=buff[1];

    temp=temp<<8;

    temp=temp|buff[0];

    return temp;

}


//ds18b20_major.h
#ifndef _DS18B20_H_
#define _DS18B20_H_

/**DS18B20 FUNCTION DECLARATION**/
unsigned char ResetDS18b20(void);
unsigned char ReadBit(void);
void WriteBit(unsigned char);
unsigned char ReadByte(void);
void WriteByte(unsigned char);
int ReadTemp(void);

#endif

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
        else
        {
            StrLCD("FORWARD DRIVE MODE");

            Buzzer_OFF();
        }

        delay_ms(300);
    }
}




