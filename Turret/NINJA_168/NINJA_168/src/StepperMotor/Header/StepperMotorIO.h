#ifndef _STEPPER_MOTOR_IO_H
#define _STEPPER_MOTOR_IO_H

extern unsigned int leftQuarterStep(void);
extern unsigned int rightQuarterStep(void);
extern unsigned int panSteppingInDirection(unsigned char direction);

extern unsigned int downQuarterStep(void);
extern unsigned int upQuarterStep(void);
extern unsigned int tiltSteppingInDirection(unsigned char direction);

#endif
