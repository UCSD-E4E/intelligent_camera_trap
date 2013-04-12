#include "HyperVisor.h"

static unsigned char hibernateSend = FALSE;
static unsigned char hibernateAcknowledge = FALSE;

unsigned char getHibernateAcknowledge(void)
{
    return hibernateAcknowledge;
}

unsigned char getHibernateSend(void)
{
    unsigned char tempHolder;
    tempHolder = hibernateSend;
    hibernateSend = FALSE;
    return tempHolder;
}

void setHibernateAcknowledge(unsigned char cmd)
{
    hibernateAcknowledge = cmd;
}

void sendHibernateRequest(void)
{
    hibernateSend = TRUE;
}

