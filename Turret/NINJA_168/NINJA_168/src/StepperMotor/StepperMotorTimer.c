#include "./Header/StepperMotorTimer.h"
#include "./Header/constant.h"

static unsigned int currentPanStepPeriod = PAN_SLOWEST_QUARTER_STEP_IN_2MS;
static unsigned int panTimer = 0;

void setCurrentPanStepPeriod(unsigned int timerSetting){
    if (timerSetting >= PAN_FASTEST_QUARTER_STEP_IN_2MS && timerSetting <= PAN_SLOWEST_QUARTER_STEP_IN_2MS) {
        currentPanStepPeriod = timerSetting;
    }
}

void resetPanSpeed(void)
{
  currentPanStepPeriod = PAN_SLOWEST_QUARTER_STEP_IN_2MS;
}

unsigned int getCurrentPanStepPeriod()
{
  return currentPanStepPeriod;
}

void panAccelerate(void)
{
    if (currentPanStepPeriod > PAN_FASTEST_QUARTER_STEP_IN_2MS)
    {
      currentPanStepPeriod--;
    }
}

void initPanQuarterStepTimer(void)
{
    panTimer = PAN_SLOWEST_QUARTER_STEP_IN_2MS;
}

void resetPanQuarterStepTimer(void)
{
    panTimer = currentPanStepPeriod;
}

void setPanQuarterStepTimer(unsigned int timerSetting){
    panTimer = timerSetting;
}

unsigned char expiredPanQuarterStepTimer(void){

    return (panTimer == 0) ;
}

void decrementPanQuarterStepTimer(void){

    if (panTimer > 0) {
        panTimer--;
    }
}

unsigned int getPanQuarterStepTimer(void){

    return panTimer;

}

//Tilt


static unsigned int currentTiltStepPeriod = TILT_SLOWEST_QUARTER_STEP_IN_2MS;
static unsigned int tiltTimer = 0;

void setCurrentTiltStepPeriod(unsigned int timerSetting){
    if (timerSetting >= TILT_FASTEST_QUARTER_STEP_IN_2MS && timerSetting <= TILT_SLOWEST_QUARTER_STEP_IN_2MS) {
        currentTiltStepPeriod = timerSetting;
    }
}

void resetTiltSpeed(void)
{
  currentTiltStepPeriod = TILT_SLOWEST_QUARTER_STEP_IN_2MS;
}

unsigned int getCurrentTiltStepPeriod()
{
  return currentTiltStepPeriod;
}

void tiltAccelerate(void)
{
    if (currentTiltStepPeriod > TILT_FASTEST_QUARTER_STEP_IN_2MS)
    {
      currentTiltStepPeriod--;
    }
}

void initTiltQuarterStepTimer(void)
{
    tiltTimer = TILT_SLOWEST_QUARTER_STEP_IN_2MS;
}

void resetTiltQuarterStepTimer(void)
{
    tiltTimer = currentTiltStepPeriod;
}

void setTiltQuarterStepTimer(unsigned int timerSetting){
    tiltTimer = timerSetting;
}

unsigned char expiredTiltQuarterStepTimer(void){

    return (tiltTimer == 0) ;
}

void decrementTiltQuarterStepTimer(void){

    if (tiltTimer > 0) {
        tiltTimer--;
    }
}

unsigned int getTiltQuarterStepTimer(void){

    return tiltTimer;

}





