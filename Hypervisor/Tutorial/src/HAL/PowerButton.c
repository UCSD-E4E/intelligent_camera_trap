#include "HyperVisor.h"
#include "port.h"

static unsigned char currentPowerButton = OFF;

unsigned char getPowerButtonPressed(void)
{
	return currentPowerButton;
}

void powerButtonPressed(unsigned char command)
{
	relayON2(command);
}
