#include "HyperVisor.h"
#include <avr/interrupt.h>
#include "port.h"



#define MISSING_HEARTBEAT_TIME_THRESHOLD 30
#define MAXIMUM_HEARTBEAT_TIME 3600


volatile static unsigned int heart_beat_inactive_time_in_S = MISSING_HEARTBEAT_TIME_THRESHOLD;
volatile static unsigned char heart_beat_inactive_time_in_100MS = 0;



unsigned char get_bbb_heart_beat(void)
{
	return (heart_beat_inactive_time_in_S < MISSING_HEARTBEAT_TIME_THRESHOLD);
}

void set_bbb_heart_beat(unsigned char currentHearBeat)
{
	if (currentHearBeat == ON)
	{

		cli();
		heart_beat_inactive_time_in_S = 0;
		heart_beat_inactive_time_in_100MS = 0;
		sei();
	}
	
	else if (currentHearBeat == OFF)
	{
		cli();
		heart_beat_inactive_time_in_S = MISSING_HEARTBEAT_TIME_THRESHOLD;
		heart_beat_inactive_time_in_100MS = 0;
		sei();
	}
}

unsigned int get_bbb_heart_beat_inactive_time_in_S(void)
{
	return heart_beat_inactive_time_in_S;
}


void increment_bbb_heart_beat_inactive_time_100MS(void)
{
	if (heart_beat_inactive_time_in_S <= MAXIMUM_HEARTBEAT_TIME)
	{
		heart_beat_inactive_time_in_100MS++;
		
		if (heart_beat_inactive_time_in_100MS == 10)
		{
			heart_beat_inactive_time_in_S++;
			heart_beat_inactive_time_in_100MS = 0;
		}
	}
}

unsigned char readBBBHeartBeatLine(void)
{
	return inputON3();
}
