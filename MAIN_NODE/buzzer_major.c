// buzzer_major.c

#include <LPC21xx.h>
#include "buzzer_major.h"

#define BUZZER 20    // Buzzer pin

// Initialize buzzer
void Buzzer_init(void)
{
    // Set as output
    IODIR0 |= (1 << BUZZER);

    // Turn OFF buzzer
    IOCLR0 |= (1 << BUZZER);
}

// Turn ON buzzer
void Buzzer_ON(void)
{
    IOCLR0 |= (1 << BUZZER);
}

// Turn OFF buzzer
void Buzzer_OFF(void)
{
    IOSET0 |= (1 << BUZZER);
}