#include "HyperVisor.h"

static unsigned char tracking = OFF;
static unsigned char trackingInactiveTimeInS = 0;

unsigned char getTracking(void){

    return tracking;
}

unsigned char getTrackingInactiveTimeInS(void){

    return trackingInactiveTimeInS;
}

void setTracking(unsigned char currentTracking){

    tracking = currentTracking;

    if (tracking == ON) {

        trackingInactiveTimeInS = 0;
    }
}

void setTrackingInactiveTimeInS(unsigned char currentTrackingInactiveTimeInS){

    trackingInactiveTimeInS = currentTrackingInactiveTimeInS;
}
