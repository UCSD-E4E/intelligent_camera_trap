#ifndef _STEPPER_MOTOR_STM_H
#define _STEPPER_MOTOR_STM_H

extern unsigned char panSpeedAdjustment(unsigned char direction);
extern void panStepMotorSTM(void) ;
extern unsigned char getPanMoving(void);
extern unsigned char getPanDirection(void);
extern void initPanPositionToCenter(void);

extern unsigned char tiltSpeedAdjustment(unsigned char direction);
extern void tiltStepMotorSTM(void) ;
extern unsigned char getTiltMoving(void);
extern unsigned char getTiltDirection(void);
extern void initTiltPositionToCenter(void);

#endif
