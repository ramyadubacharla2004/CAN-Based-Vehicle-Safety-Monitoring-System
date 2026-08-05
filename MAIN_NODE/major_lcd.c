// major_lcd.c

#include <LPC21xx.h>
#include "major_lcd_defines.h"
#include "major_type.h"
#include "defines_major.h"
#include "delay_major.h"
#include "major_lcd.h"

// Write data to LCD
void WriteLCD(u8 data)
{
    // Write mode
    SCLRBIT(IOCLR0,RW);

    // Put data on bus
    WBYTE(IOPIN0,LCD_DATA,data);

    // Enable pulse
    SSETBIT(IOSET0,EN);
    delay_us(1);
    SCLRBIT(IOCLR0,EN);

    // LCD delay
    delay_ms(2);
}

// Send command
void CmdLCD(u8 cmd)
{
    // Command mode
    SCLRBIT(IOCLR0,RS);

    WriteLCD(cmd);
}

// Send character
void CharLCD(u8 ascii)
{
    // Data mode
    SSETBIT(IOSET0,RS);

    WriteLCD(ascii);
}

// Initialize LCD
void InitLCD(void)
{
    // LCD data pins output
    WBYTE(IODIR0,LCD_DATA,255);

    // Control pins output
    SETBIT(IODIR0,RS);
    SETBIT(IODIR0,RW);
    SETBIT(IODIR0,EN);

    // LCD initialization
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

// Display string
void StrLCD(s8 *p)
{
    while(*p)
        CharLCD(*p++);
}

// Display unsigned number
void U32LCD(u32 n)
{
    u8 a[10];
    s32 i = 0;

    if(n == 0)
    {
        CharLCD('0');
    }
    else
    {
        // Convert to digits
        while(n)
        {
            a[i++] = (n % 10) + 48;
            n /= 10;
        }

        // Display digits
        for(--i; i >= 0; i--)
            CharLCD(a[i]);
    }
}

// Display signed number
void S32LCD(s32 n)
{
    if(n < 0)
    {
        CharLCD('-');
        n = -n;
        U32LCD(n);
    }
}

// Display float
void F32LCD(f32 fnum, u8 nDP)
{
    u32 n;
    s32 i;

    if(fnum < 0)
    {
        CharLCD('-');
    }

    n = fnum;
    U32LCD(n);

    CharLCD('.');

    // Display decimal part
    for(i = 0; i < nDP; i++)
    {
        fnum = (fnum - n) * 10;
        n = fnum;
        CharLCD(n + 48);
    }
}

// Display hexadecimal
void HexLCD(u32 n)
{
    u8 a[8], rem;
    s32 i = 0;

    if(n == 0)
    {
        CharLCD('0');
    }
    else
    {
        // Convert to hex
        while(n)
        {
            rem = n % 16;

            (rem < 10) ? (rem += 48) : (rem += 55);

            a[i++] = rem;

            n /= 16;
        }

        // Display hex value
        for(--i; i >= 0; i--)
        {
            CharLCD(a[i]);
        }
    }
}

// Display binary
void BinLCD(u32 n, u8 nbd)
{
    s32 i;

    // Display bits
    for(i = nbd - 1; i >= 0; i--)
    {
        CharLCD(((n >> i) & 1) + 48);
    }
}

// Load custom characters
void BuildCGRAM(u8 *p, u8 nb)
{
    s32 i;

    // Select CGRAM
    CmdLCD(GOTO_CGRAM);

    // Write pattern
    for(i = 0; i <= nb; i++)
    {
        CharLCD(p[i]);
    }

    // Return to LCD
    CmdLCD(GOTO_LINE2_POS0);
}