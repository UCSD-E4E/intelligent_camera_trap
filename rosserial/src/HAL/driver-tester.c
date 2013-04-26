/*
 * driver_tester.c
 *
 * Created: 12/12/2012 12:57:03 AM
 *  Author: Samuel
 */ 

#include "avr/io.h"
#include "port.h"
#include "uart.h"
#include "MessagePrint.h"
#include "HAL.h"
#include "MessageTX.h"
#include "driver-tester.h"

void tester_0();
void tester_1();
void tester_2();
void tester_3();
void tester_4();
void tester_5();
void tester_6();
void tester_7();
void tester_8();
void tester_9();
void tester_10();
void tester_11();

void (*tester[TEST_CASE_NUMBER]) (void) = 
{	tester_0, tester_1, tester_2, tester_3, tester_4, 
	tester_5, tester_6, tester_7, tester_8, tester_9, 
	tester_10, tester_11};

//Do nothing function
void tester_0(){UART_Transmit('A');} 

void tester_1()
{
	powerButtonPressed(1);
}

void tester_2()
{
	powerButtonPressed(0);
}

void tester_3()
{
	powerSupplyON(1);
}

void tester_4()
{
	powerSupplyON(0);
}

//Power Status
void tester_5() 
{
 UART_Transmit('P');
 UART_Transmit('B');
 
 if(isPowerLED_ON())
 {
	 UART_Transmit('1');
	 
 }else
 {
	 UART_Transmit('0'); 
 }	

 UART_Transmit(13);
 UART_Transmit(10);
}

void tester_6()
{
 UART_Transmit('P');
 UART_Transmit('B');
 
 if(isPowerLED_ON())
 {
	 UART_Transmit('1');
	 
 }else
 {
	 UART_Transmit('0');
 }

 UART_Transmit(13);
 UART_Transmit(10);
}

void tester_7()
{
	UART_Transmit('P');
	UART_Transmit('S');
	
	if(inputON2())
	{
		UART_Transmit('1');
		
	}else
	{
		UART_Transmit('0');
	}

	UART_Transmit(13);
	UART_Transmit(10);
}

void tester_8()
{
	UART_Transmit('P');
	UART_Transmit('S');
	
	if(inputON2())
	{
		UART_Transmit('1');
		
	}else
	{
		UART_Transmit('0');
	}

	UART_Transmit(13);
	UART_Transmit(10);
}

void tester_9()
{
	char sample[]="Welcome!\n";
	resetMessageBuffer();
	setMessage(sample,9);
	sendOneCharInMessage();
}


void tester_10()
{
	sendOneCharInMessage();
}	

void tester_11()
{
	while(sendOneCharInMessage());
}
