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

char printTxt[40];

extern const char version [];

/*************************************** W R I T E *****************************************/

//return Length of string
unsigned char Length(char Temp[40])
{
	unsigned char L=0;
	while (Temp[L]) L++;
	return L;
}

//print text
void Print(char Text[40])
{
	unsigned char Len, i, T;
	Len = Length(Text);
	for (i=0; i<Len; i++)
	{
		T = Text[i];
		UART_Transmit(T);
	}
	strcpy(Text, "");
}

//print text and new line
void PrintLn(char Text[40])
{
	unsigned char Len, i, T;
	Len = Length(Text);
	for (i=0; i<Len; i++)
	{
		T = Text[i];
		setTimerIn4ms(3);
		while(!timerExpired());
		UART_Transmit(T);
		
	}
	
	setTimerIn4ms(3);
	while(!timerExpired());
	UART_Transmit(13);
	
	setTimerIn4ms(3);
	while(!timerExpired());
	UART_Transmit(10);
	
}

//print new line
void PrintLine()
{
	setTimerIn4ms(3);
	while(!timerExpired());
	UART_Transmit(13);
	
	setTimerIn4ms(3);
	while(!timerExpired());
	UART_Transmit(10);
	
}

//print welcome message
void Print_Welcome()
{
	PrintLine();
	strcpy(printTxt, "***********"); PrintLn(printTxt);
	strcpy(printTxt, "Welcome    "); PrintLn(printTxt);
	strcpy(printTxt, version      ); PrintLn(printTxt);
	strcpy(printTxt, "***********"); PrintLn(printTxt);
}

//print error message
void Print_Error()
{
	unsigned char T1 = 0;
	PrintLine();
	strcpy(printTxt, "ERROR!");
	PrintLn(printTxt);
	PrintLine();
}
