import time, os, sqlite3, commands
import FSM_Class as fsm
import TIME_currentTime as ct
import thread_vision
import ninja
import Adafruit_BBIO.GPIO as GPIO

#STATES
STATE_INIT          =  "STATE_INIT"       
STATE_PHONE_HOME    =  "STATE_PHONE_HOME"
STATE_SPAWN_VISION  =  "STATE_SPAWN_VISION"
STATE_LOGIC         =  "STATE_LOGIC"  
STATE_SHUT_DOWN     =  "STATE_SHUT_DOWN"  
              
currentTime  = ct.getCurrentTime()

HEART_BEAT = "P8_14"
SHUTDOWN = "P8_13"
GPIO.setup(HEART_BEAT  , GPIO.OUT)
GPIO.setup(SHUTDOWN    , GPIO.OUT)
GPIO.output(HEART_BEAT , GPIO.LOW)
GPIO.output(SHUTDOWN   , GPIO.LOW)
counter = 100
invalid_counter = 0

def heart_beat() :
    GPIO.output("P8_14", GPIO.LOW)
    time.sleep(0.25)
    GPIO.output("P8_14", GPIO.HIGH)
    time.sleep(0.25)
    GPIO.output("P8_14", GPIO.LOW)

def shut_down() :
    GPIO.output(SHUTDOWN, GPIO.HIGH)
    time.sleep(0.5)
    GPIO.output(SHUTDOWN, GPIO.LOW)

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
    global counter
    global invalid_counter
    counter = counter + 1
    angle, validity = thread_vision.getAngle_Validity()
    print "Angle: %s" %(angle)
    print "Validity: %s" %(validity)
    location = 960 - ( 900 * angle / 180 )    
    if validity == 1 :
        ninja.panX(location) 
        invalid_counter = 0
    else :
       ninja.center()
       invalid_counter = invalid_counter + 1

    if counter > 30 :
        heart_beat()
        counter = 0
    else:
        time.sleep(0.5)

    if invalid_counter > 20 :
        return STATE_SHUT_DOWN
    else :
        return STATE_LOGIC
    
def SF_SHUT_DOWN ():
    print "SF_SHUT_DOWN"
    shut_down()
    time.sleep(60)
    while True:
        print "SF_SHUT_DOWN"
        time.sleep(60)
        
    return STATE_SHUT_DOWN    
