#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

void power_off_components()
{
	//make sure the watchdog is off
		
	/* Write logical one to WDTOE and WDE */
	WDTCR |= (1<<WDTOE) | (1<<WDE);
	/* Turn off WDT */
	WDTCR = 0x00;
}

/*
* "Power-down" sleep mode.
*/
void power_down_sleep_mode()
{
	// IN THE FUTURE ALSO TURN OFF THE JTAG (set JTD bit in MCUCSR to 1)... for now, we'll leave JTAG on out of convenience
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode();
	
}