/*
* hypervisor_jan_2014.c
*
* Created: 1/15/2014 3:28:22 PM
* Author: David Muller
* Revision : Muhsin Gurel 01/22/2014
*/


#define OSCSPEED 16*1000*1000 /* in Hz */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <string.h>
#include "avr/io.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>


#include "HAL/Headers/HyperVisor.h"
#include "HAL/Headers/port.h"
#include "HAL/Headers/uart.h"
#include "HAL/Headers/MessagePrint.h"
#include "HAL/Headers/shutdown.h"
#include "HAL/Headers/state.h"
#include "HAL/Headers/bbb.h"
#include "HAL/Headers/bbb_heartbeat.h"
#include "HAL/Headers/go_pro.h"
#include "HAL/Headers/Timer.h"

/*********************************** INTERRUPT ********************************/

volatile unsigned int counter = 0;
// Timer
ISR(TIMER0_OVF_vect)
{
	if (counter < 3)
	{
		counter++;
	}
	else
	{
		counter = 0;
		increment_bbb_heart_beat_inactive_time_100MS();
	}
}

//Interrupt Service Routine for INT0...Trailmaster
ISR(INT0_vect)
{
	set_state(ON);
}
/********************************************************************************/


/*
* Set up INT0 for Trailmaster interrupt.
*/
void set_up_interrupt_pin()
{
	GICR |= 1<<INT0;                // Enable INT0
	MCUCR &= ~(1<<ISC01) ;        // Trigger INT0 on falling edge
	MCUCR &= ~(0<<ISC00) ;
	sei();                                //Enable Global Interrupt
}


/*********************************** MAIN ********************************/
int main(void)
{
	int current_state;
	unsigned char old_value;
	unsigned char new_value;
	
	// initialize input/output gpios, UART, and the timer
	PORT_Init();
	UART_Init(57600);
	TIMER_Init();
	
	// make sure all extra components (watchdog,...) are off
	power_off_components();
	
	// set up INT0 for Trailmaster input
	set_up_interrupt_pin();
	
	while(1)
	{
		current_state = get_state();
		
		if (current_state == ON)
		{
			PrintLn("ON");
			turn_on_bbb();
			turn_on_gopro(); // requires a 3 second GPIO hold, so we turn on GoPro after flipping the BBB relay
			set_bbb_heart_beat(ON);
			_delay_ms(30000);  //let Ubuntu boot up
			old_value = readBBBHeartBeatLine();
			
			while(check_if_bbb_is_off() && get_bbb_heart_beat() )
			{
				new_value = readBBBHeartBeatLine();
				
				if (old_value != new_value) 

				{
					set_bbb_heart_beat(ON);

				}
			}


			set_state(OFF);
			turn_off_bbb();
			turn_off_gopro(); // again, GoPro requires a 3 second GPIO hold, so do after the BBB
		}
		
		else if (current_state == OFF)
		{
			PrintLn("OFF!");
			power_down_sleep_mode();
			
		}
	}
}
