/*
 * write.c
 *
 * Created: 12/11/2012 11:03:46 PM
 *  Author: Samuel
 */ 
#include <string.h>
#include "avr/io.h"
#include "uart.h"
#include "Timer.h"
#include "./StepperMotor/Header/constant.h"

#define MAX_MESSAGE_SIZE 32
static unsigned char length,index;
static char message[MAX_MESSAGE_SIZE];

//print text and new line
unsigned char sendOneCharInMessage()
{
	if (length >= 1)
	{
		length--;
		setTimerIn4ms(3);
		while(!timerExpired());	
		UART_Transmit(message[index]);
		index++;
		return TRUE;
	}
	else
	{
		index = 0;
		return FALSE;		
	}
}

unsigned char sendEntireMessage()
{
    while (sendOneCharInMessage());
}
void setMessage(char* stringData, unsigned char stringLength)
{
	length = stringLength;
	memcpy(message,stringData,stringLength);
}

void addOneChar(char data)
{
	if (length < MAX_MESSAGE_SIZE)
		message[length++] = data; 
}

void resetMessageBuffer()
{
	length = 0;
	index = 0;
}
