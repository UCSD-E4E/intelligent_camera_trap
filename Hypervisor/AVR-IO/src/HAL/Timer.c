#include "avr/io.h"
#include <avr/interrupt.h>

static unsigned int timer = 0;

void setTimerIn100ms(unsigned int timerSetting){
	cli();
    timer = timerSetting;
	sei();
}
void decrementTimerBy100ms(void){

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
	TCCR0 = (1<<CS02)|(1<<CS00);	// Timer Clock = system clock / 1024
	TIFR  = 1<<TOV0;				// Clear TOV0 / Clear pending interrupts
	TIMSK = 1<< TOIE0;				// Enable Timer 0 Overflow Interrupt
}
