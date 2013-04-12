#include "HyperVisor.h"
#include "port.h"

#define POWERLEDON_TIME_THRESHOLD   50
#define MAXIMUM_POWERLEDON_TIME     200

static unsigned char powerLEDStatus= FALSE;
static unsigned char counter = 0;

unsigned char isPowerLED_ON(void)
{
	//return powerLEDStatus;
    return (counter >= POWERLEDON_TIME_THRESHOLD);
}

void setPowerLEDStatus(unsigned char powerLED)
{
	powerLEDStatus = powerLED;
}

void checkPowerLED(void)
{
    if (inputON2()) {

        //10 seconds ON
        if (counter < MAXIMUM_POWERLEDON_TIME)
            counter++;
    }else
    {
        counter = 0;
    }
}
