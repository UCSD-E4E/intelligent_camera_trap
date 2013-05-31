#include "avr/io.h"
#include <avr/interrupt.h>

static unsigned int timer = 0;

extern void setTimerIn4ms( unsigned int timerSetting )
{
	cli();
	timer = timerSetting;
	sei();
}
extern void decrementTimerBy4ms( void )
{

	if (timer > 0) {
		timer--;
	}
}
unsigned int getTimerIn100ms(void){

	return timer;

}
unsigned char timerExpired(){

	return timer == 0;
}

void TIMER_Init(void)
{
	TCCR0B = (1<<CS01)|(1<<CS00);	// Timer Clock = system clock / 64
	TIFR0  = 1<<TOV0;				// Clear TOV0 / Clear pending interrupts
	TIMSK0 = 1<< TOIE0;				// Enable Timer 0 Overflow Interrupt
}
