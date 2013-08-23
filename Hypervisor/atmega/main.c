/*
 * main.c
 *
 *  Created on: July 9, 2013
 *  Author: Gabrielle Trotter
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

/*****FUNCTIONS*****/
void set_interrupt_pins();	//initialize the registers that control the INT0 pin
void low_pwr_sleep();		//put the system into a low power sleep state
void init(); 				//initialize registers for low power consumption
void wait_for_shutdown();	//wait for the signal LED from the beaglebone to go off.
void start_camera();		//set up GPIO pins to start the spycam
void stop_camera();			//turn off spycam, reset GPIO pins.


/*****MAIN*****/
int main(void) {
	init();
	while (1) {
		low_pwr_sleep();
	}
}


/*****INTERRUPT ROUTINES*****/
/*
 * Interrupt service routine for pin INT0
 * (connected to the trailmaster signal)
 */
ISR(INT0_vect) {
	sleep_disable();
	cli();	//disable interrupts

	//Pin D5 is an output (to relay) and output is HIGH
	DDRD |= _BV(DDD5);
	PORTD |= _BV(PORTD5);

	start_camera();
	wait_for_shutdown();	//Wait for BBB to be done tracking
	stop_camera();

	//for optimal power reduction, D5 is again an input and pull-up resistor is disabled.
	DDRD &= ~(_BV(DDD5));
	PORTD &= ~(_BV(PORTD5));

	_delay_ms(200);

	sei();	//enable interrupts
}


/*****FUNCTION DEFINITIONS*****/

/*
 * This function sets up registers related to
 * GPIO port D pins in order for INT0 to work.
 * INT0 should be connected to the signal
 * input from the trailmaster. (white wire, PTR aux jack)
 */
void set_interrupt_pins() {
	//set pin D2 (INT0) to an input, pull-up resistor disabled.
	DDRD &= ~(_BV(DDD2));
	PORTD &= ~(_BV(PORTD2));

	/* set INT0 edge trigger
	 * 0 = low level, 1 = any, 2 = rising, 3 = falling
	 * Right now, set for rising edge. Trailmaster gives a pulse (~4.5V),
	 * so this should be fine.
	 */
	EICRA &= 0xF2;
	EICRA |= 0x02;

	// Turn on INT0
	EIMSK |= (1 << INT0);

}

/*
 * Sleep function - Puts CPU into power-down sleep mode
 * This is the lowest power sleep mode
 * MCU can be woken up by interrupts or a restart.
 */
void low_pwr_sleep() {
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sei(); //IMPORTANT must be done before sleeping or it will only wake up on restart.

	// turn on brown-out enable select (allows us to turn off brown out detector)
	MCUCR = _BV (BODS) | _BV (BODSE);
	MCUCR = _BV (BODS); // keep this right below the previous - turns off brown out detector

	sleep_mode();
}

/*
 * Initializes CPU for low power:
 * 	 -All GPIO ports set to input
 * 	 -Internal Pull-up resistors deactivated
 * 	 -ADC turned off
 * 	 -Analog comparator disabled
 * 	 -Power saving features turned on
 * 	 -Disable digital input buffer on analog input pins
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
 * Halt everything and wait for
 * an external input from pin D4.
 * This function uses polling,
 * could possibly be re-implemented to
 * sleep the ATMEGA until it is time to
 * shut down the BBB. (using some sort
 * of I/O interrupt)
 */
void wait_for_shutdown() {
	_delay_ms(400);

	//D4 is an input with pull-up resistor enabled (from BBB power LED)
	DDRD &= ~(_BV(DDD4));
	PORTD |= (_BV(PORTD4));

	//Wait for the BBB pwr LED to turn off
	char value = PIND & (1 << PIND4);
	while (value) {
		value = PIND & (1 << PIND4);
	}

	_delay_ms(200);
}

/*
 * Powers on the camera and begins recording.
 */
void start_camera(){
	DDRB |= _BV(DDB1);	//GPIO pin to camera power
	DDRB |= _BV(DDB2);	//GPIO pin to camera record button
	PORTB |= _BV(PORTB2);

	//Hold power button for 2 seconds
	PORTB &= ~(_BV(PORTB1));
	_delay_ms(2000);
	PORTB |= _BV(PORTB1);

	_delay_ms(1500); //Don't change - Unreliable at lower values

	//Press record button (doesn't require a hold)
	PORTB &= ~(_BV(PORTB2));
	_delay_ms(200);
	PORTB |= _BV(PORTB2);
}

/*
 * Ends recording and shuts down camera.
 * Didn't thoroughly experiment with timing in here because
 * the start-up time is more critical than the shut-down
 * time.
 */
void stop_camera(){
	//Press record button for 2 seconds
	PORTB &= ~(_BV(PORTB2));
	_delay_ms(200);
	PORTB |= _BV(PORTB2);

	_delay_ms(1000);

	//Press power button twice for 2 seconds
	PORTB &= ~(_BV(PORTB1));
	_delay_ms(2000);
	PORTB |= _BV(PORTB1);
	_delay_ms(100);
	PORTB &= ~(_BV(PORTB1));
	_delay_ms(2000);
	PORTB |= _BV(PORTB1);

	//Back to input, pull-up disabled.
	DDRB &= ~(_BV(DDB1));
	PORTB &= ~(_BV(PORTB1));
	DDRB &= ~(_BV(DDB2));
	PORTB &= ~(_BV(PORTB2));

}

