#include "HyperVisor.h"
#include "HyperVisorCommand.h"
#include "MessageTX.h"

static unsigned char powerOffSend = FALSE;
static unsigned char powerOffAcknowledge = FALSE;

unsigned char getPowerOffAcknowledge(void)
{
	unsigned char powerOffAcknowledgeTemp = powerOffAcknowledge;
	powerOffAcknowledge = FALSE;
	
    return powerOffAcknowledgeTemp;
}

void setPowerOffAcknowledge(unsigned char cmd)
{
	powerOffAcknowledge = cmd;
}

void sendPowerOffRequest(void)
{
	prepareHyperVisorCommand(HV_CMD_POWEROFF);
    sendEntireMessage();
}

