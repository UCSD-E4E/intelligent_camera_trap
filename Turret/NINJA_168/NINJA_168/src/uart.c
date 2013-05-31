#define	__AVR_ATmega168__	1
#define OSCSPEED	8000000		/* in Hz */

#include <string.h>
#include "avr/io.h"
#include "./StepperMotor/Header/constant.h"

/*************************************** U A R T *****************************************/

void UART_Init(uint32_t Baud)
{
	unsigned int BaudRate = OSCSPEED / (16 * Baud) - 1;		//calculate BaudRate from page 145

	//set BaudRate into registers
	UBRR0H = (unsigned char) BaudRate>>8;
	UBRR0L = (unsigned char) BaudRate;	

	UCSR0A = (0<<U2X0); 
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);		// RXEN & TXEN enable (Bits 4, 3 = 1)
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);	// set Frame Format 8 bits per frame (Bit1 = 1, Bit2 = 1), 1 stop bit(Bit3 =0)		
}

unsigned char UART_Receive()
{
	if (UCSR0A & 0b10000000)
		return UDR0;
	else
		return 0;
}

unsigned char UART_ReceiveSuccess(char* data)
{
	if (UCSR0A & 0b10000000)
	{
		*data = UDR0;
		return TRUE;
	}else
	{		
		return FALSE;	
	}
}

void UART_Transmit(unsigned char Data)
{
	while (!(UCSR0A & 0b00100000));
	UDR0 = Data;
}