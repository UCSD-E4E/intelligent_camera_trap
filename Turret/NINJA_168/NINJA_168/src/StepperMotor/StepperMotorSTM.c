#include "./Header/StepperMotorSTM.h"
#include "./Header/StepperMotorTimer.h"
#include "./Header/StepperMotorIO.h"
#include "./Header/StepperMotorPosition.h"
#include "./Header/Constant.h"

unsigned char panSpeedAdjustment(unsigned char direction)
{
    static unsigned char directionHistory [4] = {STAY_PUT,STAY_PUT,STAY_PUT,STAY_PUT};
    unsigned char accelerate = SPEED_SLOWEST;

    directionHistory[3] = directionHistory[2] ;
    directionHistory[2] = directionHistory[1] ;
    directionHistory[1] = directionHistory[0] ;
    directionHistory[0] = direction ;

    if ((direction != STAY_PUT) && 
        (directionHistory[3] == directionHistory[2]) &&
        (directionHistory[2] == directionHistory[1]) &&
        (directionHistory[1] == directionHistory[0]))
    {
      accelerate = SPEED_ACCELERATE;

      if (directionHistory[0] == MOVING_LEFT) {
        directionHistory[0] = MOVING_LEFT_ACC;
      } 
      else if (directionHistory[0] == MOVING_RIGHT) 
      {
        directionHistory[0] = MOVING_RIGHT_ACC;
      }
    }
    else if ((directionHistory[0] == MOVING_LEFT  && (directionHistory[1] == MOVING_LEFT_ACC || directionHistory[1] == MOVING_LEFT )) ||
             (directionHistory[0] == MOVING_RIGHT  && (directionHistory[1] == MOVING_RIGHT_ACC || directionHistory[1] == MOVING_RIGHT )))
    {
             accelerate = SPEED_CONSTANT;
    }
    else 
    {
        accelerate = SPEED_SLOWEST;
    }
    
    return accelerate;
}

static unsigned char panMoving = FALSE;
static unsigned char panDirection = STAY_PUT;

unsigned char getPanMoving(void)
{
    return panMoving;
} 

unsigned char getPanDirection(void)
{
    return panDirection;
}

void panStepMotorSTM(void)
{
    unsigned char doneMoving = FALSE;
    unsigned char speedDecision = SPEED_CONSTANT;
     
    decrementPanQuarterStepTimer();

    if(expiredPanQuarterStepTimer())
    {

        if (panMoving == TRUE) {

            doneMoving = panSteppingInDirection(panDirection);

            if (doneMoving == TRUE) {

                panDirection = nextPanDirection();
                speedDecision = panSpeedAdjustment(panDirection);

                if (panDirection != STAY_PUT) {

                    panMoving = TRUE;
                     
                    if (speedDecision == SPEED_ACCELERATE) {
                       panAccelerate();
                    }
                    else if (speedDecision == SPEED_SLOWEST) {
                        resetPanSpeed();
                    }

                    panSteppingInDirection(panDirection);
                }
                else
                {
                    panMoving = FALSE; 
                    resetPanSpeed();
                }
            }
        }else
        {
            //Should we be moving?
            panDirection = nextPanDirection();
            speedDecision = panSpeedAdjustment(panDirection);

            if (panDirection != STAY_PUT) {

                   panMoving = TRUE; 
                   panSteppingInDirection(panDirection);
            }
        }

        resetPanQuarterStepTimer();
    }    
}

void initPanPositionToCenter()
{
    unsigned char position; 
      
   position = checkPanPosition();
   position = checkPanPosition();
   position = checkPanPosition();

   if (position == POSITION_LEFT) {
   
       setCurrentPanPosition(LEFT_LIMIT);    
   }
   else if (position == POSITION_RIGHT) {

       setCurrentPanPosition(RIGHT_LIMIT);    
   }

   setTargePanPosition(CENTER_PAN);
}

//Tilt 


unsigned char tiltSpeedAdjustment(unsigned char direction)
{
    static unsigned char directionHistory [4] = {STAY_PUT,STAY_PUT,STAY_PUT,STAY_PUT};
    unsigned char accelerate = SPEED_SLOWEST;

    directionHistory[3] = directionHistory[2] ;
    directionHistory[2] = directionHistory[1] ;
    directionHistory[1] = directionHistory[0] ;
    directionHistory[0] = direction ;

    if ((direction != STAY_PUT) && 
        (directionHistory[3] == directionHistory[2]) &&
        (directionHistory[2] == directionHistory[1]) &&
        (directionHistory[1] == directionHistory[0]))
    {
      accelerate = SPEED_ACCELERATE;

      if (directionHistory[0] == MOVING_DOWN) {
        directionHistory[0] = MOVING_DOWN_ACC;
      } 
      else if (directionHistory[0] == MOVING_UP) 
      {
        directionHistory[0] = MOVING_UP_ACC;
      }
    }
    else if ((directionHistory[0] == MOVING_DOWN  && (directionHistory[1] == MOVING_DOWN_ACC || directionHistory[1] == MOVING_DOWN )) ||
             (directionHistory[0] == MOVING_UP  && (directionHistory[1] == MOVING_UP_ACC || directionHistory[1] == MOVING_UP )))
    {
             accelerate = SPEED_CONSTANT;
    }
    else 
    {
        accelerate = SPEED_SLOWEST;
    }
    
    return accelerate;
}

static unsigned char tiltMoving = FALSE;
static unsigned char tiltDirection = STAY_PUT;

unsigned char getTiltMoving(void)
{
    return tiltMoving;
} 

unsigned char getTiltDirection(void)
{
    return tiltDirection;
}

void tiltStepMotorSTM(void)
{
    unsigned char doneMoving = FALSE;
    unsigned char speedDecision = SPEED_CONSTANT;
     
    decrementTiltQuarterStepTimer();

    if(expiredTiltQuarterStepTimer())
    {

        if (tiltMoving == TRUE) {

            doneMoving = tiltSteppingInDirection(tiltDirection);

            if (doneMoving == TRUE) {

                tiltDirection = nextTiltDirection();
                speedDecision = tiltSpeedAdjustment(tiltDirection);

                if (tiltDirection != STAY_PUT) {

                    tiltMoving = TRUE;
                     
                    if (speedDecision == SPEED_ACCELERATE) {
                       tiltAccelerate();
                    }
                    else if (speedDecision == SPEED_SLOWEST) {
                        resetTiltSpeed();
                    }

                    tiltSteppingInDirection(tiltDirection);
                }
                else
                {
                    tiltMoving = FALSE; 
                    resetTiltSpeed();
                }
            }
        }else
        {
            //Should we be moving?
            tiltDirection = nextTiltDirection();
            speedDecision = tiltSpeedAdjustment(tiltDirection);

            if (tiltDirection != STAY_PUT) {

                   tiltMoving = TRUE; 
                   tiltSteppingInDirection(tiltDirection);
            }
        }

        resetTiltQuarterStepTimer();
    }    
}

void initTiltPositionToCenter()
{
    unsigned char position; 
      
   position = checkTiltPosition();
   position = checkTiltPosition();
   position = checkTiltPosition();

   if (position == POSITION_DOWN) {
   
       setCurrentTiltPosition(DOWN_LIMIT);    
   }
   else if (position == POSITION_UP) {

       setCurrentTiltPosition(UP_LIMIT);    
   }

   setTargeTiltPosition(CENTER_TILT);
}
