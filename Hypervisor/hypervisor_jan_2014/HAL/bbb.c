#include "HyperVisor.h"
#include "avr/io.h"
#include "port.h"


void turn_on_bbb(void)
{
	relayON4(ON);
}


void turn_off_bbb(void)
{
	relayON4(OFF);
}


unsigned char check_if_bbb_is_on(void)
{
	return inputON1();
}
