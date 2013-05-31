#include <avr/interrupt.h>
#include <port.h>
#include "./Header/constant.h"
#include "./Header/StepperMotorPosition.h"
#include "./Header/StepperMotorSTM.h"

volatile static unsigned int currentPanPosition = 0 ;
volatile static unsigned int setTargetPanPosition = 0 ;

void setPanPositionToCenter(void){

    currentPanPosition = CENTER_PAN;
}

void setCurrentPanPosition(unsigned int position)
{
    if ( position >= LEFT_LIMIT && position <= RIGHT_LIMIT ) {
        currentPanPosition = position;
    }
}

unsigned int getCurrentPanPosition(void)
{
    return currentPanPosition;
}

void setTargePanPosition(unsigned int position)
{
	
    if ( position >= LEFT_LIMIT && position <= RIGHT_LIMIT ) {
        setTargetPanPosition = position;
    } 
	
	//else if (position > RIGHT_LIMIT) {
    //    setTargetPanPosition = RIGHT_LIMIT;
    //} else if (position < LEFT_LIMIT) {
    //    setTargetPanPosition = LEFT_LIMIT;
    //}
}

unsigned char nextPanDirection(void)
{
    unsigned char direction = STAY_PUT;

    if (currentPanPosition < setTargetPanPosition) {

        direction = MOVING_RIGHT;

    }
    else if (currentPanPosition > setTargetPanPosition)
    {

        direction = MOVING_LEFT;
    }

    return direction;
}

void setCurrentPanPositionLeftOneStep(void)
{
   if (!leftLimitReached()) {
       currentPanPosition--;
   }
}

void setCurrentPanPositionRightOneStep(void)
{
   if (!rightLimitReached()) {
       currentPanPosition++;
   }
}

unsigned char leftLimitReached(void)
{
  return ( currentPanPosition == LEFT_LIMIT ) ;
}

unsigned char rightLimitReached(void)
{
  return ( currentPanPosition == RIGHT_LIMIT ) ;
}


unsigned char checkPanPosition(void)
{
    static unsigned char panSensor [3] = {POSITION_UNDEFINED,POSITION_UNDEFINED,POSITION_UNDEFINED};

    unsigned char position = POSITION_UNDEFINED;

    panSensor[2] = panSensor[1] ; 
    panSensor[1] = panSensor[0] ; 
    panSensor[0] = panSensorInput();

    if ((panSensor[0] == POSITION_LEFT  && panSensor[1] == POSITION_LEFT && panSensor[2] == POSITION_RIGHT) || 
        (panSensor[0] == POSITION_RIGHT && panSensor[1] == POSITION_RIGHT  && panSensor[2] == POSITION_LEFT))
    {
        position = POSITION_CENTER_PAN;

    }
    else if (panSensor[0] == POSITION_RIGHT  && panSensor[1] == POSITION_RIGHT && panSensor[2] == POSITION_RIGHT) {

        position = POSITION_RIGHT;
    } 
    else if (panSensor[0] == POSITION_LEFT  && panSensor[1] == POSITION_LEFT && panSensor[2] == POSITION_LEFT) {

        position = POSITION_LEFT;
    }
  
    return position;
}

unsigned char panSensor = POSITION_UNDEFINED;

unsigned char getPanSensor(void)
{
   return panSensorInput();
}

//Tilt

volatile static unsigned int currentTiltPosition = 0 ;
volatile static unsigned int setTargetTiltPosition = 0 ;

void setTiltPositionToCenter(void){

    currentTiltPosition = CENTER_TILT;
}

void setCurrentTiltPosition(unsigned int position)
{
    if ( position >= DOWN_LIMIT && position <= UP_LIMIT ) {
        currentTiltPosition = position;
    }
}

unsigned int getCurrentTiltPosition(void)
{
    return currentTiltPosition;
}

void setTargeTiltPosition(unsigned int position)
{
	
    if ( position >= DOWN_LIMIT && position <= UP_LIMIT ) {
        setTargetTiltPosition = position;
    } 
	
	//else if (position > UP_LIMIT) {
    //    setTargetTiltPosition = UP_LIMIT;
    //} else if (position < DOWN_LIMIT) {
    //    setTargetTiltPosition = DOWN_LIMIT;
    //}
	
	
}

unsigned char nextTiltDirection(void)
{
    unsigned char direction = STAY_PUT;

    if (currentTiltPosition < setTargetTiltPosition) {

        direction = MOVING_UP;

    }
    else if (currentTiltPosition > setTargetTiltPosition)
    {

        direction = MOVING_DOWN;
    }

    return direction;
}

void setCurrentTiltPositionDownOneStep(void)
{
   if (!downLimitReached()) {
       currentTiltPosition--;
   }
}

void setCurrentTiltPositionUpOneStep(void)
{
   if (!upLimitReached()) {
       currentTiltPosition++;
   }
}

unsigned char downLimitReached(void)
{
  return ( currentTiltPosition == DOWN_LIMIT ) ;
}

unsigned char upLimitReached(void)
{
  return ( currentTiltPosition == UP_LIMIT ) ;
}


unsigned char checkTiltPosition(void)
{
    static unsigned char tiltSensor [3] = {POSITION_UNDEFINED,POSITION_UNDEFINED,POSITION_UNDEFINED};

    unsigned char position = POSITION_UNDEFINED;

    tiltSensor[2] = tiltSensor[1] ; 
    tiltSensor[1] = tiltSensor[0] ; 
    tiltSensor[0] = tiltSensorInput();

    if ((tiltSensor[0] == POSITION_DOWN  && tiltSensor[1] == POSITION_DOWN && tiltSensor[2] == POSITION_UP) || 
        (tiltSensor[0] == POSITION_UP && tiltSensor[1] == POSITION_UP  && tiltSensor[2] == POSITION_DOWN))
    {
        position = POSITION_CENTER_TILT;

    }
    else if (tiltSensor[0] == POSITION_UP  && tiltSensor[1] == POSITION_UP && tiltSensor[2] == POSITION_UP) {

        position = POSITION_UP;
    } 
    else if (tiltSensor[0] == POSITION_DOWN  && tiltSensor[1] == POSITION_DOWN && tiltSensor[2] == POSITION_DOWN) {

        position = POSITION_DOWN;
    }
  
    return position;
}

unsigned char tiltSensor = POSITION_UNDEFINED;

unsigned char getTiltSensor(void)
{
   return tiltSensorInput();
}

