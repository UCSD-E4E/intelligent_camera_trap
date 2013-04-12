#include "HyperVisor.h"

static unsigned char suspendSend = FALSE;
static unsigned char suspendAcknowledge = FALSE;

unsigned char getSuspendAcknowledge(void)
{
    return suspendAcknowledge;
}
unsigned char getSuspendSend(void)
{
    unsigned char tempHolder;
    tempHolder = suspendSend;
    suspendSend = FALSE;
    return tempHolder;
}

void setSuspendAcknowledge(unsigned char cmd)
{
    suspendAcknowledge = cmd;
}

void sendSuspendRequest(void)
{
    suspendSend = TRUE;
}
