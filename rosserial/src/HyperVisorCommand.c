/*
 * action.c
 *
 * Created: 12/11/2012 11:10:34 PM
 *  Author: Samuel
 */ 
#include <string.h>
#include "avr/io.h"
#include "uart.h"
#include "MessagePrint.h"
#include "MessageTX.h"
#include "HyperVisorCommand.h"

const char HyperVisorRequest[HYPERVISOR_COMMAND_NUMBER][HYPERVISOR_COMMAND_BYTE_NUMBER] = {"HC1X", "HC1S","HC1H","HC1A"};

/******************************** A C T I O N S *****************************/
void prepareHyperVisorCommand(unsigned char command)
{
	unsigned char Len, i, T, checksum;
	Len = HYPERVISOR_COMMAND_BYTE_NUMBER ;
	
	resetMessageBuffer();
	//addOneChar(ASCI_CR);
	//addOneChar(ASCI_LF);

	checksum = 0;
	for (i=0; i < Len; i++)
	{
		T = HyperVisorRequest[command][i];
		checksum += T;	
		addOneChar(T);
	}
	checksum = 0 - checksum;
	//addOneChar(checksum);
}