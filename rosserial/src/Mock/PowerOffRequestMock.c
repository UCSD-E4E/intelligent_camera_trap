#include "HyperVisor.h"

static unsigned char powerOffSend = FALSE;
static unsigned char powerOffAcknowledge = FALSE;

unsigned char getPowerOffAcknowledge(void)
{
    return powerOffAcknowledge;
}

unsigned char getPowerOffSend(void)
{
    unsigned char tempHolder;
    tempHolder = powerOffSend;
    powerOffSend = FALSE;
    return tempHolder;
}

void setPowerOffAcknowledge(unsigned char cmd)
{
    powerOffAcknowledge = cmd;
}

void sendPowerOffRequest(void)
{
    powerOffSend = TRUE;
}

