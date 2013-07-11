/*
 * main.c
 *
 *  Created on: July 9, 2013
 *      Author: Gabrielle Trotter
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

void set_interrupt_pins();
void blink_5_times();
void low_pwr_sleep();

int main(void) {
	set_interrupt_pins();

	//set only pin 0 of port B as output (1)


	while (1) {

		blink_5_times();
		low_pwr_sleep();

	}
}

void set_interrupt_pins() {
	DDRD &= 0xFD; // Clear the PD2 pin
	// PD2 (PCINT0 pin) is now an input

	/*
	 * PORTD |= _BV(PORTD2);    // turn on the Pull-up
	 *PD2 is now an input with pull-up enabled
	 *not sure this is necessary because of the pulse we are getting
	 *from trailmaster stays low then goes high...
	 */

	// set INT0 edge trigger
	// 0 = low level, 1 = any, 2 = rising, 3 = falling
	EICRA &= 0xF2;
	EICRA |= 0x02;

	// Turn on INT0
	EIMSK |= (1 << INT0);

	// turn on global interrupts
}

//Interrupt service routine for pin INT0
ISR(INT0_vect) {
	sleep_disable();
	PORTB = 0x00;
	_delay_ms(2000);

}

void blink_5_times() {
	DDRB = 0xFF;
	int x = 10;
	while (x) {
		PINB = _BV(PINB0);
		_delay_ms(500);
		x--;

	}

	PORTB = 0x01;
}

void low_pwr_sleep() {
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_bod_disable();
	sei();
	sleep_mode();
}

