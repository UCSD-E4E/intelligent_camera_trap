/*
 * write.c
 *
 * Created: 12/11/2012 11:03:46 PM
 *  Author: Samuel
 */ 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "avr/io.h"
#include "avr/interrupt.h"
#include "uart.h"
#include "./StepperMotor/Header/constant.h"
#include "./StepperMotor/Header/StepperMotorPosition.h"
#include "./StepperMotor/Header/StepperMotorSTM.h"
#include "MessageRX.h"
#include "MessageTX.h"

typedef void (*AppPtr_t)(void) __attribute__ ((noreturn));
AppPtr_t AppStartPtr = (AppPtr_t) 0x3800;

char receivedMessage[40];
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
			receivedMessage[byteNumber - HEADER_LENGTH]	= temp;		
			byteLeft--;
		}
		//Bypass Checksum
		//else if (checkSum == 0)
		else
		{
			receiveMessageLength = byteNumber + 1;
			receiving = FALSE;
			previousChar = 0;			
			return MESSAGE_TO_PARSE;
		}
		/*
		else
		{
			receiving = FALSE;
			previousChar = 0;
			return NO_MESSAGE_TO_PARSE;			
		}
		*/		
		byteNumber++;
		return NO_MESSAGE_TO_PARSE;
	}		
}

void parseMessageTemp(void)
{
	char message [32];
	unsigned int xPosition ;
	unsigned int yPosition ;	
	unsigned int targetXPosition;
	unsigned int targetYPosition;
	
	cli();
	xPosition = getCurrentPanPosition();
	yPosition = getCurrentTiltPosition();
	sei();
	
	
	if (messageType == CONTROL_MESSAGE)
	{
		if (receivedMessage[0] == '0')
		{
			setTargePanPosition(xPosition + 1 );
			
		}else if (receivedMessage[0] == '1')
		{
			setTargePanPosition(xPosition - 1 );
			
		}else if (receivedMessage[0] == '2')
		{
			setTargeTiltPosition(yPosition + 1 );
			
		}else if (receivedMessage[0] == '3')
		{
			setTargeTiltPosition(yPosition - 1 );
			
		}else if (receivedMessage[0] == '4')
		{
			setTargePanPosition(xPosition + 50);
			
		}else if (receivedMessage[0] == '5')
		{
			setTargePanPosition(xPosition - 50);
			
		}else if (receivedMessage[0] == '6')
		{
			setTargeTiltPosition(yPosition + 50);
							
		}else if (receivedMessage[0] == '7')
		{
			setTargeTiltPosition(yPosition - 50);
				
		}else if (receivedMessage[0] == '8')
		{
			setTargePanPosition(RIGHT_LIMIT);
		}
		else if (receivedMessage[0] == '9')
		{
			setTargePanPosition(LEFT_LIMIT);
		} 
		else if (receivedMessage[0] == 'a')
		{
			setTargeTiltPosition(UP_LIMIT);
			
		}
		else if (receivedMessage[0] == 'b')
		{
			setTargeTiltPosition(DOWN_LIMIT);
						
		}
		else if (receivedMessage[0] == 'c')
		{
			initPanPositionToCenter();
			
		}else if (receivedMessage[0] == 'd')
		{
			initTiltPositionToCenter();
		
		}else if (receivedMessage[0] == 'e')
		{
			AppStartPtr();		
		}		
	}else if (messageType == QUERY)
    {
	     sprintf(message,"X:%i Y:%i\r\nLR:%i DU:%i\r\n",xPosition,yPosition,getPanSensor(),getTiltSensor());
		 //sprintf(message,"X:%i Y:%i\r\n",xPosition,yPosition);
		 setMessage(message,strlen((const char*)message));		
    
	}else if (messageType == DATA_MESSAGE)
    {
		targetXPosition = atoi(&receivedMessage[1]);
		targetYPosition = atoi(&receivedMessage[6]);
		cli();
		if (receivedMessage[0] == 'X') 
		{
			setTargePanPosition(targetXPosition);
		}else if (receivedMessage[0] == 'R')
		{
			targetXPosition = xPosition + targetXPosition;
			setTargePanPosition(targetXPosition);
		}else if (receivedMessage[0] == 'L')
		{
			targetXPosition = xPosition - targetXPosition;
			setTargePanPosition(targetXPosition);
		}
		
		if (receivedMessage[5] == 'Y')
		{
			setTargeTiltPosition(targetYPosition);
		}else if (receivedMessage[5] == 'U')
		{
			targetYPosition = yPosition + targetYPosition;
			setTargeTiltPosition(targetYPosition);
		}else if (receivedMessage[5] == 'D')
		{
			targetYPosition = yPosition - targetYPosition;
			setTargeTiltPosition(targetYPosition);
		}
		sei();
		
		//sprintf(message,"Target X:%i Y:%i\r\nLR:%i DU:%i\r\n",targetXPosition,targetYPosition,getPanSensor(),getTiltSensor());
		sprintf(message,"Target X:%i Y:%i\r\n",targetXPosition,targetYPosition);
		setMessage(message,strlen((const char*)message));
		     					
	}	
}