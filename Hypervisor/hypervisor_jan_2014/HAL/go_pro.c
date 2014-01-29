#include "HyperVisor.h"
#include "avr/io.h"
#include "port.h"
#include <util/delay.h>

void turn_on_gopro(void)
{
	relayON2(ON);
	_delay_ms(3000);
	relayON2(OFF);
}


void turn_off_gopro(void)
{
	relayON2(ON);
	_delay_ms(3000);
	relayON2(OFF);
}

