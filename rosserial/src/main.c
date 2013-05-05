#define	__AVR_ATmega16__	1
#define OSCSPEED	16*1000*1000		/* in Hz */

#include <string.h>
#include "avr/io.h"
#include <avr/interrupt.h>

#include "HyperVisor.h"
#include "HyperVisorCommand.h"

#include "port.h"
#include "uart.h"
#include "MessagePrint.h"
#include "MessageRX.h"

#include "Timer.h"
#include "Activity.h"
#include "Tracking.h"
#include "HeartBeat.h"
#include "PowerLED.h"

#include "driver-tester.h"
#include "StateFunctions.h"

/*********************************** INTERRUPT ********************************/
volatile unsigned int counter = 0;

ISR(TIMER0_OVF_vect)
{
	if (counter < 3)
		{
		counter++;
		}
	else
		{
		counter = 0;
		decrementTimerBy100ms();
		checkActivity();
		incrementTrackingInactiveTimeBy100MS();
		incrementHeartBeatInactiveTimeBy100MS();
		checkPowerLED();
		}
}

/*********************************** MAIN ********************************/

int main()
{
	unsigned char State;
	unsigned char pastState;

	char txtString[24];
	const char stateStr[5][20] = { "POWEROFF", "BOOTING", "RUN", "SUSPEND", "BEFORPOWEROFF" };

	PORT_Init();
	UART_Init(57600);

	State = getInitialState();
	pastState = State;

	TIMER_Init();
	sei();

    //Delay 1 Seconds
	setTimerIn100ms(10);
	while (getTimerIn100ms());

	strcpy(txtString, " "); PrintLn(txtString);
	strcpy(txtString, " "); PrintLn(txtString);
	strcpy(txtString, "CURRENT STATE:"); PrintLn(txtString);
	PrintLn(stateStr[State]);

	while (1)
		{
		//STM
		State = Ubuntu_State[State]();

		//Force to another state
		State = parseMessageTemp(State);

		if (State != pastState)
			{
			pastState = State;
			PrintLn(stateStr[State]);

			//Delay 1 Second -- No too fast state transitions
			setTimerIn100ms(10);
			while (getTimerIn100ms());
			}
		}

	return 0;
}
