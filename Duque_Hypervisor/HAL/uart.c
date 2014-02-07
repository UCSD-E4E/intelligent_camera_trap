#define	__AVR_ATmega16__	1
#define OSCSPEED	16000000		/* in Hz */

#include <string.h>
#include "Headers/HyperVisor.h"
#include "avr/io.h"

/*************************************** U A R T *****************************************/

void UART_Init(uint32_t Baud)
{
	unsigned int BaudRate = OSCSPEED / (16 * Baud) - 1;		//calculate BaudRate from page 145

	//set BaudRate into registers
	UBRRH = (unsigned char) BaudRate>>8;
	UBRRL = (unsigned char) BaudRate;
	

	UCSRB = UCSRB | 0b00011000;		// RXEN & TXEN enable (Bits 4, 3 = 1)
	UCSRC = UCSRC | 0b10000110;		// set Frame Format 8 bits per frame (Bit1 = 1, Bit2 = 1), 1 stop bit(Bit3 =0)
}

unsigned char UART_Receive()
{
	if (UCSRA & 0b10000000)
		return UDR;
	else
		return 0;
}

unsigned char UART_ReceiveSuccess(char* data)
{
	if (UCSRA & 0b10000000)
	{
		*data = UDR;
		return TRUE;
	}else
	{		
		return FALSE;	
	}
}

void UART_Transmit(unsigned char Data)
{
	while (!(UCSRA & 0b00100000));
	UDR = Data;
}