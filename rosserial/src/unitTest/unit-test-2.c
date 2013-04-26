#include "./unity/unity.h"
#include "unit-test-2.h"
#include "StateFunctions.h"
//#include "PowerMock.h"
//#include "HeartBeatMock.h"
//#include "TrackingMock.h"
#include "HyperVisor.h"
//#include "PowerOffRequestMock.h"

unsigned char State;
extern unsigned char timerCountDownActivated;;

//Start and stay at PowerOFF
void test_stateTransition0(void)
{
  //All of these should pass
  State = getInitialState();
  State = Ubuntu_State[State]();
  TEST_ASSERT_EQUAL(POWEROFF,State);
  State = Ubuntu_State[State]();
  TEST_ASSERT_EQUAL(POWEROFF,State);
}

//Activity triggers the poweroff to turn power on
void test_stateTransition1(void)
{
    //Initial state
    //TEST_ASSERT_EQUAL(OFF,activityDetected());
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(POWEROFF,State);

    //Activity Detected but not processed yet
    //setActivityDetected(TRUE);
    //TEST_ASSERT_EQUAL(TRUE,activityDetected());
    //TEST_ASSERT_EQUAL(FALSE,getCurrentPowerSwitch());

    //Activity Detected and processed 
    State = Ubuntu_State[State]();
    //TEST_ASSERT_EQUAL(ON,activityDetected());
    //TEST_ASSERT_EQUAL(ON,getCurrentPowerSwitch());
    TEST_ASSERT_EQUAL(POWEROFF,State);
}


//Transitioning to turn the power on state
void test_stateTransition2(void)
{

    //Initial Power has been detected ON
    //But timer not expired yet
    //setPowerStatus(ON);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(POWEROFF,State);
    //setPowerStatus(OFF);

    //Timer expires
    //setTimerIn100ms(0);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(POWEROFF,State);

    //Initial Power has been detected ON
    //setPowerStatus(ON);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(BOOTING,State);
}

//Booting
//Waiting for Heart Beat
void test_stateTransition3(void)
{
    //Initial Booting
    //TEST_ASSERT_EQUAL(0,getHeartBeatInactiveTimeInS());
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(BOOTING,State);

    //Done Booting
    //setHeartBeat(ON);
    //TEST_ASSERT_EQUAL(ON,getHeartBeat());
    //TEST_ASSERT_EQUAL(0,getHeartBeatInactiveTimeInS());
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(RUN,State);
}

//Run can go to Power OFF
//Run can go to SUSPEND
void test_stateTransition4(void)
{
    //Initial Run
    TEST_ASSERT_EQUAL(RUN,State);
    //TEST_ASSERT_EQUAL(ON,getHeartBeat());
    //TEST_ASSERT_EQUAL(0,getHeartBeatInactiveTimeInS());
    //TEST_ASSERT_EQUAL(OFF,getTracking());
    //TEST_ASSERT_EQUAL(0,getTrackingInactiveTimeInS());
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(RUN,State);

    //In the process of going to POWEROFF
    //setHeartBeat(OFF);
    //setHeartBeatInactiveTimeInS(HEARTBEAT_THRESOLD_SEC-1);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(RUN,State);

    //setHeartBeatInactiveTimeInS(HEARTBEAT_THRESOLD_SEC);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(RUN,State);

    //Run -> POWEROFF
    //setHeartBeatInactiveTimeInS(HEARTBEAT_THRESOLD_SEC + 1);
    //TEST_ASSERT_EQUAL(OFF,getCurrentPowerSwitch());
    TEST_ASSERT_EQUAL(NOT_ACTIVATED,timerCountDownActivated);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(POWER_BUTTON,timerCountDownActivated);
    TEST_ASSERT_EQUAL(RUN,State);
    //TEST_ASSERT_EQUAL(ON,getCurrentPowerSwitch());

    //setTimerIn100ms(ZERO);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(RUN,State);
    //TEST_ASSERT_EQUAL(OFF,getCurrentPowerSwitch());
    //setPowerStatus(OFF);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(POWEROFF,State);
    
    //Go back to Run
    //setHeartBeat(ON);
    State = Ubuntu_State[RUN]();
    TEST_ASSERT_EQUAL(RUN,State);
    //TEST_ASSERT_EQUAL(OFF,getCurrentPowerSwitch());

    //In the process of going to Suspend
    //TEST_ASSERT_EQUAL(OFF,getTracking());
    //TEST_ASSERT_EQUAL(0,getTrackingInactiveTimeInS());
    State = Ubuntu_State[RUN]();
    TEST_ASSERT_EQUAL(RUN,State);

    //setTracking(OFF);
    //setTrackingInactiveTimeInS(SUSPEND_THRESOLD_SEC-1);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(RUN,State);

    //setTrackingInactiveTimeInS(SUSPEND_THRESOLD_SEC);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(RUN,State);

    //Run -> Suspend
    //setTrackingInactiveTimeInS(SUSPEND_THRESOLD_SEC + 1);
    TEST_ASSERT_EQUAL(NOT_ACTIVATED,timerCountDownActivated);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(SUSPEND_ACK_WAITING,timerCountDownActivated);
    TEST_ASSERT_EQUAL(RUN,State);

    TEST_ASSERT_EQUAL(SUSPEND_ACK_WAITING,timerCountDownActivated);
    //setTimerIn100ms(ZERO);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(NOT_ACTIVATED,timerCountDownActivated);
    TEST_ASSERT_EQUAL(RUN,State);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(SUSPEND_ACK_WAITING,timerCountDownActivated);
    //setSuspendAcknowledge(TRUE);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(SUSPEND,State);
}

//SUSPEND can go back to RUN
//SUSPEND can get ready to BEFOREHIBERNATE 
void test_stateTransition5(void)
{
    //Initial Suspend
    TEST_ASSERT_EQUAL(SUSPEND,State);
    //setActivityDetected(FALSE);
    //TEST_ASSERT_EQUAL(FALSE,activityDetected());
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(SUSPEND,State);

    //setActivityDetected(TRUE);
    //TEST_ASSERT_EQUAL(TRUE,activityDetected());
    State = Ubuntu_State[State]();
    //TEST_ASSERT_EQUAL(ON,getCurrentPowerSwitch());
    TEST_ASSERT_EQUAL(SUSPEND,State);

    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(SUSPEND,State);

    //setTimerIn100ms(ZERO);
    State = Ubuntu_State[State]();
    //TEST_ASSERT_EQUAL(OFF,getCurrentPowerSwitch());
    TEST_ASSERT_EQUAL(BOOTING,State);

    //Initial Suspend
    State = SUSPEND;
    //setActivityDetected(FALSE);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(SUSPEND,State);

    //setActivityInactiveTimeInS(HIBERNATION_THRESOLD_SEC-1);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(SUSPEND,State);

    //setActivityInactiveTimeInS(HIBERNATION_THRESOLD_SEC);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(SUSPEND,State);

    //SUSPEND -> BEFOREHIBERNATE
    //powerButtonPressed(RELEASED);
    //TEST_ASSERT_EQUAL(OFF,getCurrentPowerSwitch());
    //setActivityInactiveTimeInS(HIBERNATION_THRESOLD_SEC + 1);
    State = Ubuntu_State[State]();
    //TEST_ASSERT_EQUAL(ON,getCurrentPowerSwitch());
    TEST_ASSERT_EQUAL(SUSPEND,State);

    //setTimerIn100ms(ZERO);
    State = Ubuntu_State[State]();
    //TEST_ASSERT_EQUAL(OFF,getCurrentPowerSwitch());
    TEST_ASSERT_EQUAL(BEFOREHIBERNATE,State);
}

//BEFOREHIBERNATE can go back to HIBERNATE
void test_stateTransition6(void)
{
    //Initial BEFOREHIBERNATE
    //setPowerStatus(ON);
    TEST_ASSERT_EQUAL(BEFOREHIBERNATE,State);
    //getHibernateSend();
    //TEST_ASSERT_EQUAL(FALSE,getHibernateSend());
    TEST_ASSERT_EQUAL(NOT_ACTIVATED,timerCountDownActivated);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(HIBERNATE_ACK_WAITING,timerCountDownActivated);
    TEST_ASSERT_EQUAL(BEFOREHIBERNATE,State);
    //TEST_ASSERT_EQUAL(TRUE,getHibernateSend());

    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(BEFOREHIBERNATE,State);

    //setPowerStatus(OFF);
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(HIBERNATE,State);
}

//
void test_stateTransition7(void)
{
    //Initial HIBERNATE
    TEST_ASSERT_EQUAL(HIBERNATE,State);  
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(HIBERNATE,State);

    //setActivityDetected(TRUE);
    //powerButtonPressed(RELEASED);
    //TEST_ASSERT_EQUAL(FALSE,getCurrentPowerSwitch());
    State = Ubuntu_State[State]();
    //TEST_ASSERT_EQUAL(TRUE,getCurrentPowerSwitch());
    TEST_ASSERT_EQUAL(RUN,State);
}

