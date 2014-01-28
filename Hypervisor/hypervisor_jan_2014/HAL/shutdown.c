#include <avr/io.h>
#include <avr/interrupt.h>


void WDT_off(void)
{
	/* Write logical one to WDTOE and WDE */
	WDTCR |= (1<<WDTOE) | (1<<WDE);
	/* Turn off WDT */
	WDTCR = 0x00;
}