#include "avr/io.h"

void putIntoReset()
{
	
	PORTB = PORTB | 0b00001000;
	
	return ;
}

// O - PD2 TILT4
// O - PD3 TILT3
// O - PD5 Hijack-RESET
// O - PD6 PAN3
// O - PD7 PAN2

// O - PB0 PAN1
// O - PB1 PAN4
// I - PB2 PAN_POS_DATA
// O - PB7 TILT_POS_EN (E2)

// I - PC0 TILT_POS_DATA
// I - PC2 E3 ?
// O - PC3 TILT2
// O - PC4 TILT1

void PORT_Init()
{
	putIntoReset();
	
	PORTB = 0b00000000;
	DDRB =  0b10000011;

	PORTC = 0b00000000;
	DDRC =  0b00011000;

	PORTD = 0b00000000;
	DDRD =  0b11101110;		//set TX as output (Bit1 = 1)

}

unsigned char panSensorInput()
{
	if (PINB & 0b00000100)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned char tiltSensorInput()
{
	if (PINC & 0b00000001)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



void tiltSensor_EN(unsigned char cmd)
{
	if (cmd)
	{
		PORTB = PORTB & 0b01111111;
	}
	else
	{
		PORTB = PORTB | 0b10000000;
		
	}
	return ;
}

void panCH1(unsigned char cmd)
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

void panCH2(unsigned char cmd)
{
	if (cmd)
	{
		PORTD = PORTD | 0b10000000;
	}
	else
	{
		PORTD = PORTD & 0b01111111;
	}
	return ;
}

void panCH3(unsigned char cmd)
{
	if (cmd)
	{
		PORTD = PORTD | 0b01000000;
	}
	else
	{
		PORTD = PORTD & 0b10111111;
	}
	
	return ;
}


void panCH4(unsigned char cmd)
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

void tiltCH1(unsigned char cmd)
{
	if (cmd)
	{
		PORTC = PORTC | 0b00010000;
	}
	else
	{
		PORTC = PORTC & 0b11101111;
	}
	
	return ;
}

void tiltCH2(unsigned char cmd)
{
	if (cmd)
	{
		PORTC = PORTC | 0b00001000;
	}
	else
	{
		PORTC = PORTC & 0b11110111;
	}
	
	return ;
}

void tiltCH3(unsigned char cmd)
{
	if (cmd)
	{
		PORTD = PORTD | 0b00001000;
	}
	else
	{
		PORTD = PORTD & 0b11110111;
	}
	return ;
}

void tiltCH4(unsigned char cmd)
{
	if (cmd)
	{
		PORTD = PORTD | 0b00000100;
	}
	else
	{
		PORTD = PORTD & 0b111111011;
	}
	return ;
}


void hijack_EN(unsigned char cmd)
{
	if (cmd)
	{
		//Driving /Reset Pin Low
		PORTD = PORTD & 0b00100000;
	}
	else
	{
		
		PORTD = PORTD | 0b00100000;
	}
	return ;
}