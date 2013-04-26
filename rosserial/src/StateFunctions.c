#include "StateFunctions.h"
#include "HyperVisor.h"
#include "HAL.h"

unsigned char (*const Ubuntu_State[])(void) = {st_powerOff, st_booting, st_run, st_suspend,st_beforePowerOff}; 
unsigned char timerCountDownActivated = FALSE;

unsigned char getInitialState(void){
	
	powerSupplyON(OFF);
	return POWEROFF;
}

unsigned char stateTransition(unsigned char currentState){

    static unsigned char pastState = POWEROFF;
    unsigned char result ;

    result = (pastState == currentState);
    pastState = currentState;

    return result;
}

unsigned char st_powerOff(void){

    //State Transition is occuring 
    if (timerCountDownActivated == POWER_BUTTON_ON) {

        if (timerExpired())
        {
            powerButtonPressed(RELEASED);
            if (isPowerLED_ON()) {

                timerCountDownActivated = NOT_ACTIVATED;
                setTimerIn100ms(ZERO);
                setHeartBeat(OFF);
                return BOOTING;
            }
        }

        //May hangs here
        return POWEROFF;        
    }

    //Press the button for POWER_BUTTON_PRESSED time and switch state
    if (activityDetected()) {
		powerSupplyON(ON);		
        powerButtonPressed(PRESSED);
        timerCountDownActivated = POWER_BUTTON_ON;
        setTimerIn100ms(POWER_BUTTON_PRESSED);
    }

    return POWEROFF;
}
unsigned char st_booting(void){

    if (getHeartBeat()) {
		//Need to reset the tracking timer 
		setTracking(ON);
        setHeartBeat(ON);
		return RUN;
    }
    else
    {
        return BOOTING;
    }
}

unsigned char st_run(void){

      if (timerCountDownActivated == POWER_SUPPLY) {

         if (isPowerLED_ON()== OFF) {
				
                timerCountDownActivated = NOT_ACTIVATED;
                setTracking(OFF);
                setHeartBeat(OFF);
                setActivityDetected(FALSE);
                return POWEROFF;
         }			
        
		return RUN;        
      } 
      else if (timerCountDownActivated == SUSPEND_ACK_WAITING) {

          if (timerExpired()) {
              //Computer might hangs
              //Heartbeat = TRUE but never ACK SUSPEND
              timerCountDownActivated = NOT_ACTIVATED;
              
		 }else if (getSuspendAcknowledge()) {

              timerCountDownActivated = NOT_ACTIVATED;
              setTimerIn100ms(ZERO);
              setTracking(OFF);
              setHeartBeat(OFF);
              setActivityDetected(FALSE);
              return SUSPEND;
          }
		  
		  return RUN;
      }

    
    if (getHeartBeatInactiveTimeInS() > HEARTBEAT_THRESOLD_SEC) {
        //Ubuntu hangs -> Turn it off
        powerSupplyON(OFF);
		timerCountDownActivated = POWER_SUPPLY;
    }else if (getTracking()==OFF) {
        //Ubuntu not tracking -> Suspend
        sendSuspendRequest();
        timerCountDownActivated = SUSPEND_ACK_WAITING;
        setTimerIn100ms(SUSPENSION_WAIT);
    }

    return RUN;
}
unsigned char st_suspend(void){

    //State Transition is occuring 
    if (timerCountDownActivated == POWER_BUTTON_OFF) {

        if (timerExpired())
        {
            powerButtonPressed(RELEASED);
            timerCountDownActivated = NOT_ACTIVATED;

            if (getActivityInactiveTimeInS() > POWEROFF_THRESOLD_SEC) {

                return BEFOREPOWEROFF;
            }
            else
            {
                setHeartBeat(OFF);
                return BOOTING;
            }
        }
        else{

            return SUSPEND;
        }
    }

    //Activty detected OR NON-Activity for too long
    if (activityDetected() || getActivityInactiveTimeInS() > POWEROFF_THRESOLD_SEC) {
        powerButtonPressed(PRESSED);
        timerCountDownActivated = POWER_BUTTON_OFF;
        setTimerIn100ms(POWER_BUTTON_PRESSED);
    }

    return SUSPEND;
}
//Need to add the hangs
unsigned char st_beforePowerOff(void){

     //State Transition is occurring 
    if (timerCountDownActivated == POWEROFF_ACK_WAITING) {

        if (isPowerLED_ON() == OFF)
        {
            timerCountDownActivated = NOT_ACTIVATED;
            setTimerIn100ms(ZERO);
            setActivityDetected(FALSE);
            powerSupplyON(OFF);
            return POWEROFF;
        }
        else if (timerExpired())
        {
            timerCountDownActivated = NOT_ACTIVATED;
        }
        else
        {
            return BEFOREPOWEROFF;
        }
    }

    //Needs to send every five seconds
    sendPowerOffRequest();
    timerCountDownActivated = POWEROFF_ACK_WAITING;
    setTimerIn100ms(POWEROFF_WAIT);

    return BEFOREPOWEROFF;
}

