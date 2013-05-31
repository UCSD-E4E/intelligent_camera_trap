#define	__AVR_ATmega168__	1
#define OSCSPEED	8*1000*1000		/* in Hz */

#include <string.h>
#include "avr/io.h"
#include <avr/interrupt.h>
#include "./StepperMotor/Header/constant.h"
#include "./StepperMotor/Header/StepperMotorSTM.h"
#include "port.h"
#include "timer.h"
#include "uart.h"
#include "MessagePrint.h"
#include "MessageTX.h"
#include "MessageRX.h"

/*********************************** INTERRUPT ********************************/
volatile unsigned int counter = 0;
const char version [] = "1.00";

ISR(TIMER0_OVF_vect)
{	
	panStepMotorSTM();
	tiltStepMotorSTM();
		
	if (counter < 1)
	{
		counter++;
		
	}
	else
	{		
		decrementTimerBy4ms();
		counter = 0;
	}
	
}

/*********************************** MAIN ********************************/

int main()
{
	unsigned char State;
	unsigned char pastState;

	cli();
	PORT_Init();
    UART_Init(19200);    
	TIMER_Init();
	hijack_EN(ON);
	tiltSensor_EN(ON);
    sei();
		
	initPanPositionToCenter();
	initTiltPositionToCenter();

    Print_Welcome();
	resetMessageBuffer();

    while (1)
    {
		if (sendOneCharInMessage() == FALSE)
		{
			//If not Transferring
			if(receiveOneCharMessage() == MESSAGE_TO_PARSE)
			{			   
			   parseMessageTemp();
			}
		}		
    }
	
    return 0;
}
