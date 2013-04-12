#include "HyperVisor.h"

static unsigned char currentPowerSwitch = OFF;

unsigned char getCurrentPowerSwitch(void)
{
    return currentPowerSwitch;
}

void powerButtonPressed(unsigned char command)
{
    currentPowerSwitch = command;
}
