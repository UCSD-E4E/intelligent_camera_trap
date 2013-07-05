# MLX Viewer
# Displays the output of the MLX90620 thermal sensor in a human-friendly format
# Creates a 5 min video file of the output
# Creates a CSV file of the sensor values during the video
#
# Authors: Riley Yeakle, Samuel Chen, Ali Khodamoradi, David Muller, Gabrielle Trotter

import fnmatch
import sys
import serial
import Image
import pygame
import os
import time
from time import localtime, strftime

# Parameters
CELL_SIZE = 30
MAX_TEMP = 40
MIN_TEMP = 20
VIDEO_DIRECTORY = '/home/sealab/Videos/IRVideo/'	
VIDEO_LEN = 1
# Cell:
# Used to render sensor readings on the pygame window
# Create one per sensor pixel.
# Example:
#  myCell.render(255, myScreen)
#  > Creates a white square with side length = size pixels 
#    at the cell's x and y coordinates on the pygame screen
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
		color = [intensity, intensity, intensity]
		pygame.draw.rect(screen, color, (self.x, self.y, self.size, self.size), 0)	


# temperatureMap
# arguments: vector of temperature values, low_temp, high_temp 
# returns: vector of greyscale pixel intensities [0, 255]
# Example Use: (data = [1, 2, 3])
# temperatureMap(data, 1, 3)
# > [0, 127.5, 255]
def temperatureMap(values, low_temp, high_temp):
	scale_factor = 255.0/high_temp
	for i in range(len(values)):
		values[i] = (values[i] - low_temp) * scale_factor
		if values[i] > 255:
			values[i] = 255
		if values[i] < 0:
			values[i] = 0
	return values

## START SCRIPT
ser = serial.Serial()
rate = 0

#check for command line arguments
num_args = len(sys.argv)
if num_args == 3:
	br_str = fnmatch.filter(sys.argv, 'b*')
	port_str = fnmatch.filter(sys.argv, 'tty*')[0]
	rate = int(br_str[0].replace("b", ""));
elif num_args == 1:
	rate = 115200
	port_str = "ttyACM0"
else:	
	print ("Example Usage: python melexis_debug.py ttyACM0 b115200")
	exit()

ser.port = "/dev/" + port_str

print ("Opening port:" + ser.port + " at b" + str(rate))

#Set Serial Properties
ser.baudrate = rate
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.timeout = 2 
ser.writeTimeout = 2


row = 4
column = 16

try: 
	ser.open()

except Exception, e:
	print "Error opening port :( " + str(e)
	exit()

if ser.isOpen():
	
	imgcnt = 0
	movcnt = 0
	
	#initialize pygame display 
	pygame.init()
	white = [255,255,255]
	size = [CELL_SIZE*column, CELL_SIZE*row]
	screen = pygame.display.set_mode(size)
	screen.fill(white);
	grid = [];
	
	for i in range(column -1, -1, -1):
		for j in range(row):
			newCell = Cell(i*CELL_SIZE, (j)*CELL_SIZE, CELL_SIZE)
			grid.append(newCell)			
			
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
		

		#create initial CSV file with a new start time
		startTime = time.time()
		fileName = strftime("%m-%d-%H:%M:%S", localtime())
		csvFile = open(VIDEO_DIRECTORY + "IR_CSV_FILES/" + fileName + ".csv", 'a+')
		
		while True:		
			currentTime = time.time()
			#generate a new video file and CSV file every VIDEO_LEN minutes
			if (currentTime - startTime) >= (60. * VIDEO_LEN):
				outstr = 'ffmpeg -f image2 -r 5 -i ' + VIDEO_DIRECTORY + "imgtemp/%d.jpeg -vcodec mpeg4 -y " + VIDEO_DIRECTORY + fileName + '.mp4'
				os.system(outstr)
				clearImages = "rm " + VIDEO_DIRECTORY + 'imgtemp/*.jpeg'
				os.system(clearImages)
				imgcnt = 0
				#create new file name, new csv file, and reset start time
				fileName = strftime("%m-%d-%H:%M:%S", localtime())
				csvFile = open(VIDEO_DIRECTORY + "IR_CSV_FILES/" + fileName + ".csv", 'a+')
				startTime = time.time()
			response = ser.readline()
			if response.startswith("Ambient"):
				ambient = float(response.strip('Ambient T='))
				read_amb = True
			elif response.startswith("IR:"):
				data = response.strip("IR: ")
				read_dat = True
	
			if read_amb and read_dat:
				#transform input string into float array
				try:
					data_list = data.split()
					data_vector = map(float, data_list)
				except:
					print "Error in sensor value string to float conversion"
					continue 
				if len(data_vector) == row * column:	
					scaled_data = temperatureMap(data_vector, MIN_TEMP, MAX_TEMP)
				
					ambientTemp = str(ambient)
					csvFile.write(ambientTemp)	#start CSV file with ambient temp
					csvFile.write(',')
					for i in range(len(scaled_data)):
						grid[i].render(scaled_data[i], screen) 	#display a pixel

						value = str(data_vector[i])
						csvFile.write(value + ',') 	#print the rest of the values to the CSV file
					
					csvFile.write('\n')	#CSV new line:
	
					pygame.display.flip()
					imgcnt += 1
					pygame.image.save(pygame.display.get_surface(), VIDEO_DIRECTORY + 'imgtemp/' +str(imgcnt) + '.jpeg')

					read_amb = False
					read_dat = False 
			
	except Exception, e1:
		print "Error in communication with port: " + str(e1)

else:
	print "Cannot open serial port"	
