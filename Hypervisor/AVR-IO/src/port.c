#include "avr/io.h"

void PORT_Init()
{
	PORTA = 0b00000000;
	DDRA = 0b00000000;

	PORTB = 0b00000000;
	DDRB = 0b00001111;		//set Relay as output (Bit0:3 = 1)

	PORTC = 0b00000000;
	DDRC = 0b00000000;

	PORTD = 0b00000000;
	DDRD = 0b00000010;		//set TX as output (Bit1 = 1)
}

unsigned char inputON1()
{
	return !(PINA & 0b00000100) ;
}	

unsigned char inputON2()
{
	return !(PINA & 0b00000010) ;
}

unsigned char inputON3()
{
	return !(PINA & 0b00000001) ;
}

unsigned char inputON4()
{
	return !(PIND & 0b00000100) ;
}

void relayON1(unsigned char cmd)
{
	if (cmd)
	{
		PORTB = PORTB | 0b00000001;		
	}
	else
	{
		PORTB = PORTB & 0b11111110;
	}
	return ;
}

void relayON2(unsigned char cmd)
{
	if (cmd)
	{
		PORTB = PORTB | 0b00000010;
	}
	else
	{
		PORTB = PORTB & 0b11111101;
	}
	return ;
}

void relayON3(unsigned char cmd)
{
	if (cmd)
	{
		PORTB = PORTB | 0b00000100;
	}
	else
	{
		PORTB = PORTB & 0b11111011;
	}
	return ;
}


void relayON4(unsigned char cmd)
{
	if (cmd)
	{
		PORTB = PORTB | 0b00001000;
	}
	else
	{
		PORTB = PORTB & 0b11110111;
	}
	return ;
}

