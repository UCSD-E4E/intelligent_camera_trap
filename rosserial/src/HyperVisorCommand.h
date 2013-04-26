/*
 *
 * Created: 12/11/2012 11:11:17 PM
 *  Author: Samuel
 */ 

#ifndef HYPERVISOR_COMMAND_H
#define HYPERVISOR_COMMAND_H

//H = Hypervisor
//C = Command
//1 = Packet Number
//X, S, H = Actual Command
#define HYPERVISOR_COMMAND_NUMBER 4
#define HYPERVISOR_COMMAND_BYTE_NUMBER 4
#define HV_CMD_POWEROFF 0
#define HV_CMD_SUSPEND 1
#define HV_CMD_HIBERNATE 2
#define HV_CMD_ACKNOWLEDGE 3

#define ASCI_CR 13
#define ASCI_LF 10

extern const char HyperVisorRequest[HYPERVISOR_COMMAND_NUMBER][HYPERVISOR_COMMAND_BYTE_NUMBER];
//return 1-7 depending on the choosen action or 8 for inexistant input
extern void prepareHyperVisorCommand(unsigned char command);


#endif 