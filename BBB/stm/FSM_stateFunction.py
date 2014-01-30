import time, os, sqlite3, commands
import FSM_Class as fsm
import TIME_currentTime as ct
import thread_vision
import ninja
#STATES
STATE_INIT          =  "STATE_INIT"       
STATE_PHONE_HOME    =  "STATE_PHONE_HOME"
STATE_SPAWN_VISION  =  "STATE_SPAWN_VISION"
STATE_LOGIC         =  "STATE_LOGIC"  
STATE_SHUT_DOWN     =  "STATE_SHUT_DOWN"  
              
currentTime  = ct.getCurrentTime()


def SF_INIT ():
    print "SF_INIT"
    ninja.init("/dev/ttyO2")
    ninja.center()
    time.sleep(5)
    return STATE_PHONE_HOME

def SF_PHONE_HOME ():
    print "SF_PHONE_HOME"
    time.sleep(1)
    return STATE_SPAWN_VISION
  
def SF_SPAWN_VISION ():
    thread_vision.init()
    time.sleep(1)
    return STATE_LOGIC
  
def SF_LOGIC ():
    angle, validity = thread_vision.getAngle_Validity()
    print "Angle: %s" %(angle)
    print "Validity: %s" %(validity)
    location = 960 - ( 900 * angle / 180 )    
    if validity == 1 :
        ninja.panX(location) 
    else :
       ninja.center()

    time.sleep(0.5)
    return STATE_LOGIC
    
def SF_SHUT_DOWN ():
    print "SF_SHUT_DOWN"
    time.sleep(60)
    while True:
        print "SF_SHUT_DOWN"
        time.sleep(60)
        
    return STATE_SHUT_DOWN    
