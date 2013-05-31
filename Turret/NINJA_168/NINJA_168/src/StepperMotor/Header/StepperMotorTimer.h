#ifndef _STEPPER_MOTOR_TIMER_H
#define _STEPPER_MOTOR_TIMER_H

extern void setPanQuarterStepTimer(unsigned int timerSetting);
extern void decrementPanQuarterStepTimer(void);
extern void resetPanQuarterStepTimer(void);
extern void initPanQuarterStepTimer(void);
extern void setCurrentPanStepPeriod(unsigned int timerSetting);
extern void resetPanSpeed(void);
extern unsigned int getCurrentPanStepPeriod(void);
extern void panAccelerate(void);
extern unsigned char expiredPanQuarterStepTimer(void);
extern unsigned int getPanQuarterStepTimer(void);

extern void setTiltQuarterStepTimer(unsigned int timerSetting);
extern void decrementTiltQuarterStepTimer(void);
extern void resetTiltQuarterStepTimer(void);
extern void initTiltQuarterStepTimer(void);
extern void setCurrentTiltStepPeriod(unsigned int timerSetting);
extern void resetTiltSpeed(void);
extern unsigned int getCurrentTiltStepPeriod(void);
extern void tiltAccelerate(void);
extern unsigned char expiredTiltQuarterStepTimer(void);
extern unsigned int getTiltQuarterStepTimer(void);

#endif


