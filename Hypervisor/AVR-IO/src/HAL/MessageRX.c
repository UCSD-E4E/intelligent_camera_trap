/*
 * write.c
 *
 * Created: 12/11/2012 11:03:46 PM
 *  Author: Samuel
 */ 
#include <string.h>
#include "HyperVisor.h"
#include "avr/io.h"
#include "uart.h"
#include "Activity.h"
#include "Tracking.h"
#include "HeartBeat.h"
#include "PowerOffRequest.h"
#include "PowerLED.h"
#include "SuspendRequest.h"

#define	CONTROL_MESSAGE 'C'
#define	DATA_MESSAGE 'D'

#define	MESSAGE_TYPE	0
#define	MESSAGE_LENGTH	1
#define HEADER_LENGTH	2
#define NO_MESSAGE_TO_PARSE FALSE
#define MESSAGE_TO_PARSE TRUE
//Biggest Payload Allowed = 128 Bytes

char receivedMessage[128];
char messageType = 0;
char receiveMessageLength = 0;

unsigned char receiveOneCharMessage()
{
	static unsigned char receiving = FALSE;
	static unsigned char previousChar = 0;
	static unsigned char byteNumber = 0;
	static unsigned char byteLeft = 0;
	static unsigned char checkSum = 0;
	unsigned char temp;	
	
	if (receiving == FALSE)
	{
		if (UART_ReceiveSuccess(&temp))
		{
			//Received Something
			if (previousChar == 'S' && temp == 'T')
			{
				receiving = TRUE;
				byteNumber = 0;
				byteLeft = 0;
				checkSum = 0;				
			}else
			{
				previousChar = temp;
			}
		}
		return NO_MESSAGE_TO_PARSE;
	}
	else if (UART_ReceiveSuccess(&temp))
	{	
		checkSum += temp;
		if (byteNumber == MESSAGE_TYPE)
		{
			messageType =  temp;				
		}
		else if (byteNumber == MESSAGE_LENGTH)
		{
			byteLeft = temp;
			receiveMessageLength = temp;			
		}
		else if (byteLeft > 0)
		{
			receivedMessage[byteNumber - HEADER_LENGTH]	;		
			byteLeft--;
		}
		else if (checkSum == 0)
		{
			receiveMessageLength = byteNumber + 1;
			receiving = FALSE;
			previousChar = 0;			
			return MESSAGE_TO_PARSE;
		}
		else
		{
			receiving = FALSE;
			previousChar = 0;
			return NO_MESSAGE_TO_PARSE;			
		}		
		byteNumber++;
		return NO_MESSAGE_TO_PARSE;
	}		
}

unsigned char parseMessageTemp(unsigned char State)
{
	unsigned char testState;
	
	testState = UART_Receive();
		
	if (testState >= '0' && testState <= '5')
	{
			testState = testState - '0' ;
			return testState;
	}
		
	if (testState == 'A')
	{
		setActivityDetected(TRUE);
			
	}else if (testState == 'B')
	{
		setHeartBeat(TRUE);
			
	}else if (testState == 'C')
	{
		setTracking(TRUE);
		
	}else if (testState == 'D')
	{
		//setPowerLEDStatus(TRUE);
			
	}else if (testState == 'E')
	{
		//setPowerLEDStatus(FALSE);
		
	}else if (testState == 'F')
	{
		setSuspendAcknowledge(TRUE);
		
	}else if (testState == 'G')
	{
		//setHibernateAcknowledge(TRUE);
			
	}else if (testState == 'H')
	{
		setPowerOffAcknowledge(TRUE);			
	}
	
	return State;
}