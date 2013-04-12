#ifndef _HYPERVISOR_H
#define _HYPERVISOR_H


//******************************************************************************
// Firmware revision
//******************************************************************************
#define SWHIGH  1
#define SWLOW   0
#define FALSE   0  
#define TRUE    (!FALSE) 
#define ON      1
#define OFF      (!ON)
#define ZERO    0 
#define PRESSED 1
#define RELEASED 0

#define HEARTBEAT_THRESOLD_SEC 60
#define SUSPEND_THRESOLD_SEC 30
//#define POWEROFF_THRESOLD_SEC 3600
#define POWEROFF_THRESOLD_SEC 100
#define POWER_BUTTON_PRESSED 35
#define SUSPENSION_WAIT 100
#define POWEROFF_WAIT 100

#endif 
