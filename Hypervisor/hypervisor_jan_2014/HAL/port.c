#include "avr/io.h"

void PORT_Init()
{
	
	DDRA = 0b00000111; //set Relay as output (Bit0:2 = 1), Main Power, GoPro Power and GoPro On/Off Control Output
	PORTA = 0b00000000; 
	

	DDRB = 0b00000000;
	PORTB = 0b00000100; // INT2 pull-up Trail Master Input
	

	DDRC = 0b10000000; // BBB Power Control Output
	PORTC = 0b00000000;
	

	DDRD = 0b00000000;
	PORTD = 0b00111100;//set TX as output (Bit1 = 1) BBB Heartbeat and Shutdown Input 
	
}

unsigned char inputON1()
{
	return !(PIND & 0b0000100) ;
}

unsigned char inputON2()
{
	return !(PIND & 0b00001000) ;
}

unsigned char inputON3()
{
	return !(PIND & 0b00010000) ;
}

unsigned char inputON4()
{
	return !(PIND & 0b00000100) ;
}

void relayON1(unsigned char cmd)
{
	if (cmd)
	{
		PORTA = PORTA | 0b00000001;
	}
	else
	{
		PORTA = PORTA & 0b11111110;
	}
	return ;
}

void relayON2(unsigned char cmd)
{
	if (cmd)
	{
		PORTA = PORTA | 0b00000010;
	}
	else
	{
		PORTA = PORTA & 0b11111101;
	}
	return ;
}

void relayON3(unsigned char cmd)
{
	if (cmd)
	{
		PORTA = PORTA | 0b00000100;
	}
	else
	{
		PORTA = PORTA & 0b11111011;
	}
	return ;
}


void relayON4(unsigned char cmd)
{
	if (cmd)
	{
		PORTC = PORTC | 0b10000000;
	}
	else
	{
		PORTC = PORTC & 0b01111111;
	}
	return ;
}
