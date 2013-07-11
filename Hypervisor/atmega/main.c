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

/*****FUNCTIONS*****/
void set_interrupt_pins();
void blink_5_times();
void low_pwr_sleep();
void init();
void output_on_5s();

/*****MAIN*****/
int main(void) {
init();
	while (1) {
		output_on_5s();
		low_pwr_sleep();

	}
}

/*set up pins for INT0*/
void set_interrupt_pins() {
	DDRD &= ~(_BV(DDD2)); // Clear the PD2 pin
	// PD2 (PCINT0 pin) is now an input

	PORTD &= ~(_BV(PORTD2)); //turn off pull-up resistor
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

}

/*Interrupt service routine for pin INT0*/
ISR(INT0_vect) {
	sleep_disable();
	DDRB |= _BV(DDB0);
	PORTB &= ~(_BV(PORTB0));
	_delay_ms(2000);
	PORTB |= _BV(PORTB0);
	DDRB &= ~(_BV(PORTB0));

}

/*blinking light function*/
void blink_5_times() {
	PORTB &= ~(_BV(PORTB0));
	DDRB |= _BV(DDB0);
	int x = 10;
	while (x) {
		PINB |= _BV(PINB0);
		_delay_ms(500);
		x--;

	}

	PORTB |= _BV(PORTB0);
}

//sleep function
void low_pwr_sleep() {
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sei();
	MCUCR = _BV (BODS) | _BV (BODSE);  // turn on brown-out enable select
	MCUCR = _BV (BODS);        // this must be done within 4 clock cycles of above
	sleep_mode();
}

/*initialize pins for low power*/
void init() {
	DDRB = 0x00;
		DDRC = 0x00;
		DDRD = 0x00;
		PORTB = 0x00;
		PORTC = 0x00;
		PORTD = 0x00;
		ADCSRA = 0x00;
		PRR = 0xFF;
		set_interrupt_pins();
}

void output_on_5s(){
	DDRC |= _BV(DDC0);
	PORTC |= _BV(PORTC0);
	_delay_ms(5000);
	DDRC &= ~(_BV(DDC0));
	PORTC &= ~(_BV(PORTC0));
}
