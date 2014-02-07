#include "avr/io.h"
#include <avr/interrupt.h>

void TIMER_Init(void)
{
	TCCR0 = (1<<CS02)|(1<<CS00); // Timer Clock = system clock / 1024
	TIFR = 1<<TOV0; // Clear TOV0 / Clear pending interrupts
	TIMSK = 1<< TOIE0; // Enable Timer 0 Overflow Interrupt
}