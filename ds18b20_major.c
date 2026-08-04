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
