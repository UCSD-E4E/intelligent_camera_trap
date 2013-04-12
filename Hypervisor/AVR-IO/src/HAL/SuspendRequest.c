#include "HyperVisor.h"
#include "HyperVisorCommand.h"
#include "MessageTX.h"
static unsigned char suspendSend = FALSE;
static unsigned char suspendAcknowledge = FALSE;

unsigned char getSuspendAcknowledge(void)
{
	unsigned char suspendAnswer = suspendAcknowledge;
	
	suspendAcknowledge = FALSE;
    return suspendAnswer;
}

void setSuspendAcknowledge(unsigned char cmd)
{
    suspendAcknowledge = cmd;
}

void sendSuspendRequest(void)
{
	prepareHyperVisorCommand(HV_CMD_SUSPEND);
    sendEntireMessage();
}
