#include "./Header/StepperMotorIO.h"
#include "./Header/StepperMotorSTM.h"
#include "./Header/StepperMotorPosition.h"
#include "./Header/Constant.h"
#include "../port.h"

void panChannelsOFF()
{
    panCH1(OFF);
	panCH2(OFF);
	panCH3(OFF);
	panCH4(OFF);
}

void leftChannel1(void){

  panCH1(ON);
    
}

void leftChannel2(void){

   panCH2(ON);

}

void leftChannel3(void){

   panCH3(ON);
}

void leftChannel4(void){

   panCH4(ON);
}

void rightChannel1(void){

   panCH4(ON);
}

void rightChannel2(void){

   panCH3(ON);
}

void rightChannel3(void){

   panCH2(ON);
}

void rightChannel4(void){

   panCH1(ON);
}

unsigned int panSteppingInDirection(unsigned char direction)
{
    unsigned char result = FALSE;

    if (direction == MOVING_LEFT) {

        result = leftQuarterStep();

    }else if (direction == MOVING_RIGHT) {
    
        result = rightQuarterStep();
    }

    return result;

}

unsigned int leftQuarterStep(void)
{
    static unsigned char stepProcess = 0;
    unsigned char stepCompleted = 0;

    panChannelsOFF();

    if (stepProcess == 0) {

        leftChannel1();
        stepProcess++;

    }else if (stepProcess == 1) 
    {
        leftChannel2();
        stepProcess++;

    }else if (stepProcess == 2) 
    {
        leftChannel3();
        stepProcess++;

    }else if (stepProcess == 3) 
    {
        leftChannel4();
        stepProcess++;

    }else
    {
        stepCompleted = 1;
        stepProcess = 0;

        if ( checkPanPosition() == POSITION_CENTER_PAN )
        {
            setPanPositionToCenter();
        }else
        {
            setCurrentPanPositionLeftOneStep();
        }
    }
    
    return stepCompleted;
}

unsigned int rightQuarterStep(void)
{
    static unsigned char stepProcess = 0;
    unsigned char stepCompleted = 0;

    panChannelsOFF();

    if (stepProcess == 0) {

        rightChannel1();
        stepProcess++;

    }else if (stepProcess == 1) 
    {
        rightChannel2();
        stepProcess++;

    }else if (stepProcess == 2) 
    {
        rightChannel3();
        stepProcess++;

    }else if (stepProcess == 3) 
    {
        rightChannel4();
        stepProcess++;

    }else
    {
        stepCompleted = 1;
        stepProcess = 0;

        if ( checkPanPosition() == POSITION_CENTER_PAN )
        {
            setPanPositionToCenter();
        }else
        {
            setCurrentPanPositionRightOneStep();
        }
    }
    
    return stepCompleted;
}

//Tilt


void tiltChannelsOFF()
{
    tiltCH1(OFF);
	tiltCH2(OFF);
	tiltCH3(OFF);
	tiltCH4(OFF);
}

void downChannel1(void){

  tiltCH1(ON);
    
}

void downChannel2(void){

   tiltCH2(ON);

}

void downChannel3(void){

   tiltCH3(ON);
}

void downChannel4(void){

   tiltCH4(ON);
}

void upChannel1(void){

   tiltCH4(ON);
}

void upChannel2(void){

   tiltCH3(ON);
}

void upChannel3(void){

   tiltCH2(ON);
}

void upChannel4(void){

   tiltCH1(ON);
}

unsigned int tiltSteppingInDirection(unsigned char direction)
{
    unsigned char result = FALSE;

    if (direction == MOVING_DOWN) {

        result = downQuarterStep();

    }else if (direction == MOVING_UP) {
    
        result = upQuarterStep();
    }

    return result;

}

unsigned int downQuarterStep(void)
{
    static unsigned char stepProcess = 0;
    unsigned char stepCompleted = 0;

    tiltChannelsOFF();

    if (stepProcess == 0) {

        downChannel1();
        stepProcess++;

    }else if (stepProcess == 1) 
    {
        downChannel2();
        stepProcess++;

    }else if (stepProcess == 2) 
    {
        downChannel3();
        stepProcess++;

    }else if (stepProcess == 3) 
    {
        downChannel4();
        stepProcess++;

    }else
    {
        stepCompleted = 1;
        stepProcess = 0;

        if ( checkTiltPosition() == POSITION_CENTER_TILT )
        {
            setTiltPositionToCenter();
        }
		else
        {
            setCurrentTiltPositionDownOneStep();
        }
    }
    
    return stepCompleted;
}

unsigned int upQuarterStep(void)
{
    static unsigned char stepProcess = 0;
    unsigned char stepCompleted = 0;

    tiltChannelsOFF();

    if (stepProcess == 0) {

        upChannel1();
        stepProcess++;

    }else if (stepProcess == 1) 
    {
        upChannel2();
        stepProcess++;

    }else if (stepProcess == 2) 
    {
        upChannel3();
        stepProcess++;

    }else if (stepProcess == 3) 
    {
        upChannel4();
        stepProcess++;

    }else
    {
        stepCompleted = 1;
        stepProcess = 0;

        if ( checkTiltPosition() == POSITION_CENTER_TILT )
        {
            setTiltPositionToCenter();
        }else
        {
            setCurrentTiltPositionUpOneStep();
        }
    }
    
    return stepCompleted;
}


