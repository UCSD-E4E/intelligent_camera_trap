import serial
import time

packetNum = 11
commandCharNum = 1

commandHeader = "STC"
dataHeader = "STD"

#commandPan  = ["R0120", "R0100", "R0060", "R0040", "R0020", "R0010", "R0005", "R0000",  "L0005", "L0010", "L0020", "L0040", "L0060", "L0100", "L0120"]
commandPan  = ["R0320", "R0100", "R0060", "R0040", "R0020", "R0010", "R0005", "R0000",  "L0005", "L0010", "L0020", "L0040", "L0060", "L0100", "L020"]

commandXStep  = ["X0000","X0100", "X0200", "X0300","X0400","X0500", "X0600","X0700","X0800","X0900", "X1000"]
commandTilt = ["U0020", "Z0000", "D0020"]
commandCenter= "X0500Y0200"

ser = None

def init (port):
    global ser
    ser = serial.Serial(port, 19200, timeout=1)

def pan(number):
    ser.write(dataHeader + b'\x0B' + commandPan[number] + commandTilt [1] + "AA") 

#def panX_Set(number):
#    ser.write(dataHeader + b'\x05' + commandXStep[number] + "AA") 


def center():
    ser.write(dataHeader + b'\x0B' + commandCenter + "AA") 

def panX(num):
    number = str(num).zfill(4)
    ser.write(dataHeader + b'\x05' + "X" + number + "AA") 



