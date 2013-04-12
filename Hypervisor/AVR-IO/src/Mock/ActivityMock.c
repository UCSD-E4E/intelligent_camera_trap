#include "HyperVisor.h"

static unsigned char activity = FALSE;
static unsigned int  activityInactiveTimeInS = 0;

unsigned char activityDetected(void)
{
    return activity;
}

void setActivityDetected(unsigned char activityNow)
{
    activity = activityNow;

    if (activity == TRUE) {

        activityInactiveTimeInS = 0;
    }
}

void setActivityInactiveTimeInS(unsigned int timer)
{
    activityInactiveTimeInS = timer ;
}

unsigned int getActivityInactiveTimeInS(void)
{
    return activityInactiveTimeInS;
}
