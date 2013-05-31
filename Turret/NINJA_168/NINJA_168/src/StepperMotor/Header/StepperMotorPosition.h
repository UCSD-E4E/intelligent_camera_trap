#ifndef _STEPPER_MOTOR_POSITION_H
#define _STEPPER_MOTOR_POSITION_H

extern void setCurrentPanPosition(unsigned int position);
extern unsigned int getCurrentPanPosition(void);
extern void setPanPositionToCenter(void);
extern void setCurrentPanPositionLeftOneStep(void);
extern void setCurrentPanPositionRightOneStep(void);
extern void setTargePanPosition(unsigned int position);
extern unsigned char leftLimitReached(void);
extern unsigned char rightLimitReached(void);
extern unsigned char checkPanPosition(void);
extern unsigned char nextPanDirection(void);
extern unsigned char getPanSensor(void);
extern void setPanSensor(unsigned char sensorPos);

extern void setCurrentTiltPosition(unsigned int position);
extern unsigned int getCurrentTiltPosition(void);
extern void setTiltPositionToCenter(void);
extern void setCurrentTiltPositionDownOneStep(void);
extern void setCurrentTiltPositionUpOneStep(void);
extern void setTargeTiltPosition(unsigned int position);
extern unsigned char downLimitReached(void);
extern unsigned char upLimitReached(void);
extern unsigned char checkTiltPosition(void);
extern unsigned char nextTiltDirection(void);
extern unsigned char getTiltSensor(void);
extern void setTiltSensor(unsigned char sensorPos);

#endif
