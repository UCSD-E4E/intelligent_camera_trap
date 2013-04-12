#ifndef _STATEFUNCTIONS_H
#define _STATEFUNCTIONS_H

extern unsigned char stateTransition(unsigned char currentState);
extern unsigned char getInitialState(void);
extern unsigned char st_powerOff(void);
extern unsigned char st_booting(void);
extern unsigned char st_run(void);
extern unsigned char st_suspend(void);
extern unsigned char st_beforePowerOff(void);
extern unsigned char (*const Ubuntu_State[])(void); 
//STATE Function Pointer Constants
enum {POWEROFF = 0, BOOTING, RUN, SUSPEND,BEFOREPOWEROFF};
//Timer Functions
enum {NOT_ACTIVATED = 0, POWER_BUTTON_ON, POWER_BUTTON_OFF, POWER_SUPPLY, SUSPEND_ACK_WAITING, POWEROFF_ACK_WAITING};

#endif
