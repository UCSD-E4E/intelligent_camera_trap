#include "HyperVisor.h"
#include "avr/io.h"
#include "port.h"
#include <util/delay.h>

void turn_on_gopro(void)
{
	relayON3(ON);
	_delay_ms(500);
	relayON3(OFF);
}


void turn_off_gopro(void)
{
	relayON3(ON);
	_delay_ms(2000);
	relayON3(OFF);
}

