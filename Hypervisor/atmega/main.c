/*
 * main.c
 *
 *  Created on: Jul 9, 2013
 *      Author: Gabrielle Trotter
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT0_vect)
{
	PORTB = 0x00;
	_delay_ms(1000);


}


int main (void)
{
	DDRD &= 0xFC;     // Clear the PD2 pin
		// PD2 (PCINT0 pin) is now an input

	PORTD &= 0xFC;    // turn On the Pull-down
		// PD2 is now an input with pull-down enabled

	EICRA &= 0xF3;
	EICRA |= 0x03;
	//EICRA |= (1 << ISC01);
	//EICRA |= (1 << ISC00);    // set INT0 to trigger on a rising edge
	EIMSK |= (1 << INT0);     // Turns on INT0

		sei();                    // turn on interrupts

	DDRB |= 0xFF;
	while(1)
	{
		PORTB = 0x01;
		_delay_ms(500);
		PORTB = 0x00;
		_delay_ms(500);


	}
}
