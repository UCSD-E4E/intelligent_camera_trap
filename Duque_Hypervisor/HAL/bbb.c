#include "Headers/HyperVisor.h"
#include "avr/io.h"
#include "Headers/port.h"


void turn_on_bbb(void)
{
	relayON1(ON);
}


void turn_off_bbb(void)
{
	relayON1(OFF);
}

unsigned char check_if_bbb_is_off(void)
{
	return !(inputON1());
}


unsigned char check_if_bbb_is_on(void)
{
	return inputON1();
}

