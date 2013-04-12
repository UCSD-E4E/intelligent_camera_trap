#include <avr/interrupt.h>
#include "HyperVisor.h"

#define TRACKING_TIME_THRESHOLD     30
#define MAXIMUM_TRACKING_TIME       3600

static unsigned char tracking = OFF;
volatile static unsigned int trackingInactiveTimeInS = TRACKING_TIME_THRESHOLD;
volatile static unsigned char trackingInactiveTimeInMS = 0;

unsigned char getTracking(void){

    return (trackingInactiveTimeInS < TRACKING_TIME_THRESHOLD );
}

unsigned char getTrackingInactiveTimeInS(void){

    return trackingInactiveTimeInS;
}

void setTracking(unsigned char currentTracking){

    if (currentTracking == ON) {

        cli();
        trackingInactiveTimeInS = 0;
        trackingInactiveTimeInMS = 0;
		sei();

    } else if (currentTracking == OFF) {

        cli();
        trackingInactiveTimeInS = TRACKING_TIME_THRESHOLD;
        trackingInactiveTimeInMS = 0;
		sei();

	} 
}

void setTrackingInactiveTimeInS(unsigned char currentTrackingInactiveTimeInS){

    trackingInactiveTimeInS = currentTrackingInactiveTimeInS;
}

void incrementTrackingInactiveTimeBy100MS(void){

	if (trackingInactiveTimeInS  <= MAXIMUM_TRACKING_TIME)
	{
		trackingInactiveTimeInMS++;
		
		if (trackingInactiveTimeInMS == 10)
		{
			trackingInactiveTimeInS++;
			trackingInactiveTimeInMS = 0;
		}
	}
}
