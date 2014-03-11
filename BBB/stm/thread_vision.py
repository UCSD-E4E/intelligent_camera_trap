#!/usr/bin/python

import threading
import time
import os
import subprocess
import re

vision_thread = None
threadLock = threading.Lock()
angle = 0
validity = 1

class producer (threading.Thread):
    def __init__(self, threadID, name):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
    def run(self):
        global angle
        global validity
        print "Starting " + self.name
        proc = subprocess.Popen( '/home/ubuntu/intelligent_camera_trap/BBB/remote_360/remote_system' , shell = False, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
        while True:
            buffer = []
            for i in range (5):
                output = proc.stdout.readline()
                buffer.append(output)
            target = buffer[3]
            a = re.sub('[()\n]','',target)
            a.split('=')
            a.split('=')[1].split(',')
            (b,c) = a.split('=')[1].split(',')
            tempAngle = int(b)
            tempValidity = int(c)
            threadLock.acquire()
            angle = tempAngle
            validity = tempValidity
            threadLock.release()
            #print tempAngle
            #print tempValidity

def init():
    global vision_thread
    vision_thread = producer(1, "Vision")
    vision_thread.start()

def getAngle_Validity():
    global angle
    global validity
    threadLock.acquire()
    tempAngle = angle
    tempValidity = validity
    threadLock.release()
    return (tempAngle, tempValidity)

#class peeker (threading.Thread):
#    def __init__(self, threadID, name):
#        threading.Thread.__init__(self)
#        self.threadID = threadID
#        self.name = name
#    def run(self):
#        global angle
#        global validity
#        while True:
#            global angle
#            global validity
#            threadLock.acquire()
#            tempAngle = angle
#            tempValidity = validity
#            threadLock.release()
#            os.system("echo '%s' >> log.txt ; echo '%s' >> log.txt ;" %(tempAngle,tempValidity))
#            time.sleep(5)

# Create new threads

#thread2 = peeker(2, "Thread-2")

# Start new Threads
#thread2.start()

