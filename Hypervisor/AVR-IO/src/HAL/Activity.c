#include <avr/interrupt.h>
#include "HyperVisor.h"
#include "port.h"

#define INACTIVITY_TIME_THRESHOLD   30
#define MAXIMUM_INACTIVE_TIME       3600

static unsigned char activity = FALSE;
volatile static unsigned int activityInactiveTimeInS = INACTIVITY_TIME_THRESHOLD;
volatile static unsigned char activityInactiveTimeInMS = 0;

unsigned char activityDetected(void)
{
	return (activityInactiveTimeInS < INACTIVITY_TIME_THRESHOLD );
}

void setActivityDetected(unsigned char activityNow)
{
    if (activityNow == TRUE) {

		cli();
        activityInactiveTimeInS = 0;
		activityInactiveTimeInMS = 0;
		sei();

    }else if (activityNow == FALSE) {

		cli();
        activityInactiveTimeInS = INACTIVITY_TIME_THRESHOLD;
		activityInactiveTimeInMS = 0;
		sei();

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

void checkActivity(void){

    if (inputON1()) {

        activityInactiveTimeInS = 0;
        activityInactiveTimeInMS = 0;

    }else{

        if (activityInactiveTimeInS  <= MAXIMUM_INACTIVE_TIME)
        {
            activityInactiveTimeInMS++;
		
            if (activityInactiveTimeInMS == 10)
            {
                activityInactiveTimeInS++;
                activityInactiveTimeInMS = 0;
            }
		
        }
    }	
}
