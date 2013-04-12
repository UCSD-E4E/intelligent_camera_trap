#include "HyperVisor.h"
#include "port.h"

static unsigned char currentPowerSupply = OFF;

unsigned char getCurrentPowerSupply(void)
{
    return currentPowerSupply;
}

void powerSupplyON(unsigned char command)
{
    relayON1(command);
}
