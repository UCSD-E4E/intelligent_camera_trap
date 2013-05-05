#ifndef _AVR_TIME_H_
#define _AVR_TIME_H_

#include <stdint.h>

//void avr_time_init(void);
//uint32_t avr_time_now(void);

extern void setTimerIn100ms(unsigned int timerSetting);
extern void decrementTimerBy100ms(void);
extern unsigned int getTimerIn100ms(void);
extern unsigned char timerExpired(void);
//extern void TIMER_Init(void);
extern uint32_t avr_time_now(void);
extern void avr_time_init(void);
#endif
