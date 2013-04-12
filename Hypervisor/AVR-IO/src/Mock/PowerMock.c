#include "HyperVisor.h"

static unsigned char currentPowerStatus = OFF;

unsigned char powerStatusON(void)
{
    return currentPowerStatus;
}

void setPowerStatus(unsigned char powerStatus)
{
    currentPowerStatus = powerStatus;
}

