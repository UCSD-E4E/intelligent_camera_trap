#Display the IR output and record it into a .csv file

import numpy as np
import fnmatch
import sys
import serial
import Image
import pygame
import time
from time import gmtime, strftime
import traceback
import os






class Cell:
	x = 0
	y = 0
	size = 15
	intensity = 0;
	def __init__(self, pos_x, pos_y, size):
		self.x = pos_x
		self.y = pos_y
		self.size = size
	def render(self, intensity, screen):
		self.intensity = intensity
		col = [intensity, intensity, intensity]
		pygame.draw.rect(screen, col, (self.x, self.y, self.size, self.size), 0)	


ser = serial.Serial()
ser.port = "/dev/ttyACM0"
rate = 115200
	
print ("Opening port:" + ser.port + " at b" + str(rate))

#Set Serial Properties
ser.baudrate = rate
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.timeout = 2 
ser.writeTimeout = 2

interpolationScaling = 1
row_Original = 4
column_Original = 16
row_Effective = (row_Original - 1) * interpolationScaling + 1
column_Effective = (column_Original - 1) * interpolationScaling + 1


try: 
	ser.open()

except Exception, e:
	print "Error opening port :( " + str(e)
	exit()

if ser.isOpen():
	
	imgcnt = 0
	cell_size = 50
	
	#initialize pygame display 
	#grid[i].render(data_vector[(i % row_Effective) * column_Effective + (i / 	row_Effective)])				
	pygame.init()
	white = [255,255,255]
	size = [cell_size*column_Effective, cell_size*row_Effective]
	screen = pygame.display.set_mode(size)
	screen.fill(white);
	grid = [];
		
	#q = column_Effective - 1
	for q in range(column_Effective-1, -1, -1):
	#while (q):
		for j in range(row_Effective): 
			newCell = Cell(q*cell_size, (j)*cell_size, cell_size)
			grid.append(newCell)			
	#	q -= 1
		
	for i in range(len(grid)):
		grid[i].render(127.6, screen)
	
	pygame.display.flip()
	try:
		#clean out buffers
		ser.flushInput()
		ser.flushOutput()
			
		#make sure we read both the ambient and the data each cycle
		read_amb = False
		read_dat = False
	
		#this is the start time for our first csv and video files		
		startTime = time.time()		
		fileName = strftime("%m-%d-%H:%M:%S",gmtime())
	
		#open the .csv file
		csvFile = open("/home/riley/Desktop/David July 15/"+fileName+".csv", 'a+')
		
		while True:
			response = ser.readline()
			
			#add len(response) <= 16 to address bug where sometimes we read lines like this:
			# Ambient.6 24.5 23.7 23.8 25.4 24.4 24.7 24.6 24.5 24.9 25.1 23.2
			if response.startswith("Ambient") and len(response) <=16: 
				ambient = float(response.strip('Ambient T='))
				read_amb = True
			elif response.startswith("IR:"):
				data = response.strip("IR: ")
				read_dat = True
	
			if read_amb and read_dat:
				#we've had errors where the mbed reports back something like 33..1
				#so, make sure everything will convert to float first
				try:
					data_list = data.split()
					data_vector = map(float, data_list) 
				except:
					print "Could not convert IR data to float."					
					continue
	
				if len(data_vector) == 64: #make sure we got 64 pixels
				
					#assign colors for cells
					for i in range(len(data_vector)):
						data_vector[i] -= 10
						data_vector[i] *=  (150/(40-5))
						if data_vector[i] > 255:
							data_vector[i] = 255
						if data_vector[i] < 0:
							data_vector[i] = 0
				
					#display those colors
					for i in range(len(grid)):
						grid[i].render(data_vector[i], screen) 
	
					#write in this row of the .csv file
					#on each line we'll have: amibent_temp,pixel_1,pixel_2,pixel_3,...,pixel_64,
					ambientTemp = str(ambient)				
					csvFile.write(ambientTemp)   #start with ambient temp
					csvFile.write(',')
					for i in range(0, len(data_vector)):
						value = str(data_vector[i])					
						csvFile.write(value)
						csvFile.write(',')
			
					#we're done with this row
					csvFile.write('\n')
				
					pygame.display.flip()
					read_amb = False
					read_dat = False 
				
	except Exception, e1:
		print "Error in communication with port: " + str(e1)
  		traceback.print_exc(file=sys.stdout)  #print stack trace

else:
	print "Cannot open serial port"	


