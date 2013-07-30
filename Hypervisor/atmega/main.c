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
#include <avr/wdt.h>

/*****FUNCTIONS*****/
void set_interrupt_pins();
void blink_5_times();
void low_pwr_sleep();
void init();
void output_on_5s();
void wait_for_input();

/*****MAIN*****/
int main(void) {
	init();
	while (1) {
		low_pwr_sleep();
		wait_for_input();

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
	/*DDRB |= _BV(DDB5);
	PORTB |= _BV(PORTB5);
	_delay_ms(200);
	PORTB &= ~(_BV(PORTB5));
	DDRB &= ~(_BV(PORTB5));*/

}

/*blinking light function*/
void blink_5_times() {
	PORTB &= ~(_BV(PORTB5));
	DDRB |= _BV(DDB5);
	int x = 10;
	while (x) {
		PINB |= _BV(PINB5);
		_delay_ms(500);
		x--;

	}

	PORTB &= ~(_BV(PORTB5));
}

//sleep function
void low_pwr_sleep() {
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sei();
	MCUCR = _BV (BODS) | _BV (BODSE); // turn on brown-out enable select
	MCUCR = _BV (BODS); // this must be done within 4 clock cycles of above
	sleep_mode();
}

/*
 * Initializes CPU for low power:
 * 	All ports set to input
 * 	Internal Pull-up resistors deactivated
 * 	ADC turned off
 * 	Analog comparator disabled
 * 	Power saving features turned on
 * 	Disable digital input buffer on analog input pins
 * Additionally, interrupt pins set via set_interrupt_pins() function
 */
void init() {
	// set ports to input
	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x00;

	// disable internal pull-up resistors
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;

	//disable ADC and analog comparator and watchdog timer
	ACSR |= _BV(ACD);
	ADCSRA = 0x00;
	wdt_disable();

	//Disable digital input buffer on analog input pins
	DIDR1 |= _BV(AIN1D);
	DIDR1 |= _BV(AIN0D);
	DIDR0 |= 0x3F;

	//activate all power saving features
	PRR = 0xFF;

	//initialize interrupt pins
	set_interrupt_pins();
}

/*
 * Drive an output from pin C0 for 5s, then turn it off
 */
void output_on_5s() {
	DDRD |= _BV(DDD5);	//D5 is an output pin
	PORTD |= _BV(PORTD5);	//D5 is driven high
	_delay_ms(5000);
	DDRD &= ~(_BV(DDD5));	//D5 is no longer an output pin
	PORTD &= ~(_BV(PORTD5));	//D5 is disconnected from pullup resistor
}

/*
 * Halt everything and wait for
 * an external input from pin C1.
 * This function uses polling,
 * not positive this is the most
 * ideal solution for conserving power.
 */
void wait_for_input() {
	cli();	//disable interrupts
	DDRD |= _BV(DDD5); //pin D5 is an output
	PORTD |= _BV(PORTD5); //output pin D5 is on
	_delay_ms(400);
	DDRD &= ~(_BV(DDD4)); //pin D4 is input
	PORTD &= ~(_BV(PORTD4)); //disable pull-up resistor
	char value = PIND & (1 << PIND4); //read value from pin D4
	while (!value) {
		value = PIND & (1 << PIND4); //update read value from pin D4
	}
	while (value) {
		value = PIND & (1 << PIND4);
	}
	DDRD &= ~(_BV(DDD5));	//pin D5 is an input
	PORTD &= ~(_BV(PORTD5));	//input pin D5 has pull-up resistor disabled
	_delay_ms(200);
	sei();	//enable interrupts
}
