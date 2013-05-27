import numpy as np
#import scipy
import fnmatch
import sys
import serial
import Image
import pygame

class Cell:
	x = 0
	y = 0
	size = 30
	intensity = 0;
	def __init__(self, pos_x, pos_y, size):
		self.x = pos_x
		self.y = pos_y
		self.size = size
	def render(self, intensity):
		self.intensity = intensity
		col = [intensity, intensity, intensity]
		pygame.draw.rect(screen, col, (self.x, self.y, self.size, self.size), 0)	

#check for command line arguments
num_args = len(sys.argv)
if num_args == 3:
	br_str = fnmatch.filter(sys.argv, 'b*')
	port_str = fnmatch.filter(sys.argv, 'tty*')
else:	
	print ("Example Usage: python melexis_debug.py ttyACM0 b115200")
	exit()

ser = serial.Serial()
ser.port = "/dev/" + port_str[0]

rate = int(br_str[0].replace("b", ""));

print ("Opening port:" + ser.port + " at b" + str(rate))

#Set Serial Properties
ser.baudrate = rate
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.timeout = 2 
ser.writeTimeout = 2

try: 
	ser.open()

except Exception, e:
	print "Error opening port :( " + str(e)
	exit()

if ser.isOpen():
	
	cell_size = 30	
	
	#initialize pygame display
	pygame.init()
	white = [255,255,255]
	size = [cell_size*16, cell_size*4]
	screen = pygame.display.set_mode(size)
	screen.fill(white);
	grid = [];

	for i in range(16):
		for j in range(4):
			newCell = Cell(i*cell_size, (3-j)*cell_size, cell_size)
			grid.append(newCell)
			
	for i in range(len(grid)):
		grid[i].render(127.6)

	pygame.display.flip()
	try:
		#clean out buffers
		ser.flushInput()
		ser.flushOutput()
		
		#make sure we read both the ambient and the data each cycle
		read_amb = False
		read_dat = False
		
		while True:
			response = ser.readline()
			if response.startswith("Ambient"):
				ambient = float(response.strip('Ambient T='))
				read_amb = True
			elif response.startswith("IR:"):
				data = response.strip("IR: ")
				read_dat = True
	
			if read_amb and read_dat:
				print "Ambient: " + str(ambient)
				#transform input string into float array
				data_list = data.split()
				data_vector = map(float, data_list) 
				
				print "Data: "
				print data_vector
				
				for i in range(len(data_vector)):
					data_vector[i] -= 298
					data_vector[i] *= (255/(310-298))
					if data_vector[i] > 255:
						data_vector[i] = 255
					if data_vector[i] < 0:
						data_vector[i] = 0
				
				for i in range(len(grid)):
					grid[i].render(data_vector[i]);
				
				pygame.display.flip()
				read_amb = False
				read_dat = False 
			
	except Exception, e1:
		print "Error in communication with port: " + str(e1)

else:
	print "Cannot open serial port"	

