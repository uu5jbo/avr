/*
 */
#undef F_CPU
#define F_CPU 8000000L

#include <avr/io.h>
#include <avr/interrupt.h>

#include "peripf_ini.h"
#include "peripf_ini.c"

uint8_t state;

int main(void)
{
    Reset();

    timer_init();

// Insert code

    while(1)
    {
        PORTC ^= 1<<4;
    }


    return 0;
}

