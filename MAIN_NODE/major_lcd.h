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