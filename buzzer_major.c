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
