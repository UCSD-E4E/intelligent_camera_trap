#include "HyperVisor.h"

static unsigned char heartBeat = OFF;
static unsigned char heartBeatInactiveTimeInS = 0;


unsigned char getHeartBeat(void){

    return heartBeat;
}

unsigned char getHeartBeatInactiveTimeInS(void){

    return heartBeatInactiveTimeInS;
}

void setHeartBeat(unsigned char currentHearBeat){

    heartBeat = currentHearBeat;

    if (heartBeat == ON) {

        heartBeatInactiveTimeInS = 0;
    }
}

void setHeartBeatInactiveTimeInS(unsigned char currentHearBeatInactiveTime){

    heartBeatInactiveTimeInS = currentHearBeatInactiveTime;
}
