import numpy as np
import fnmatch
import sys
import serial
import Image
import pygame
import os

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
	
	cell_size = 30
	imgcnt = 0
	movcnt = 0
	
	#initialize pygame display 
	#grid[i].render(data_vector[(i % row_Effective) * column_Effective + (i / row_Effective)])				
	pygame.init()
	white = [255,255,255]
	size = [cell_size*column_Effective, cell_size*row_Effective]
	screen = pygame.display.set_mode(size)
	screen.fill(white);
	grid = [];
	
	for i in range(column_Effective):
		for j in range(row_Effective):
			newCell = Cell(i*cell_size, (j)*cell_size, cell_size)
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
				#transform input string into float array
				data_list = data.split()
				data_vector = map(float, data_list) 
				
				for i in range(len(data_vector)):
					data_vector[i] -= 10
					data_vector[i] *=  (150/(40-5))
					if data_vector[i] > 255:
						data_vector[i] = 255
					if data_vector[i] < 0:
						data_vector[i] = 0
				
				for i in range(len(grid)):
					grid[i].render(data_vector[i]) 
				    
					
				pygame.display.flip()
				imgcnt += 1
				pygame.image.save(pygame.display.get_surface(), '/home/viki/Videos/pyvideos/imgtemp/'+str(imgcnt)+'.jpeg')
				if imgcnt > 59:
					movcnt += 1
					outstr = 'ffmpeg -f image2 -r 5 -i /home/viki/Videos/pyvideos/imgtemp/%d.jpeg -vcodec mpeg4 -y /home/viki/Videos/pyvideos/movie'+str(movcnt)+'.mp4'
					os.system(outstr)
					imgcnt = 0

				read_amb = False
				read_dat = False 
			
	except Exception, e1:
		print "Error in communication with port: " + str(e1)

else:
	print "Cannot open serial port"	
