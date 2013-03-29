#include <avr/interrupt.h>
#include "HyperVisor.h"

#define MISSING_HEARTBEAT_TIME_THRESHOLD   180
#define MAXIMUM_HEARTBEAT_TIME          3600

volatile static unsigned int heartBeatInactiveTimeInS = MISSING_HEARTBEAT_TIME_THRESHOLD;
volatile static unsigned char heartBeatInactiveTimeInMS = 0;

unsigned char getHeartBeat(void){

    return (heartBeatInactiveTimeInS < MISSING_HEARTBEAT_TIME_THRESHOLD);
}

void setHeartBeat(unsigned char currentHearBeat){

	if (currentHearBeat == ON) {

        cli();
		heartBeatInactiveTimeInS = 0;
		heartBeatInactiveTimeInMS = 0;
		sei();

    } else if (currentHearBeat == OFF) {

        cli();
		heartBeatInactiveTimeInS = MISSING_HEARTBEAT_TIME_THRESHOLD;
		heartBeatInactiveTimeInMS = 0;
		sei();
    }
}

unsigned char getHeartBeatInactiveTimeInS(void){

    return heartBeatInactiveTimeInS;
}

void setHeartBeatInactiveTimeInS(unsigned char currentHearBeatInactiveTime){

    heartBeatInactiveTimeInS = currentHearBeatInactiveTime;
}

void incrementHeartBeatInactiveTimeBy100MS(void){

	if (heartBeatInactiveTimeInS  <= MAXIMUM_HEARTBEAT_TIME) 
	{
		heartBeatInactiveTimeInMS++;
		
		if (heartBeatInactiveTimeInMS == 10)
		{
			heartBeatInactiveTimeInS++; 
			heartBeatInactiveTimeInMS = 0;
		}		
	}
}
