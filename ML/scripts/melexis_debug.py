#!/usr/bin/env python
# Grab the 64 pixel "temperatures" from MLX90620. 
# "-s" command line argument will save a .csv file, and time-stamped pygame frames 
# (.csv file will be: amibent_temp,pixel_1, pixel_2, pixel_3,...,pixel_64,time_stamp)
# directories for these files modifiable at beginning at script.


import numpy as np
import fnmatch
import sys
import serial
import Image
import pygame
import time
from time import localtime, strftime
import traceback
import os





class Cell:
	x = 0
	y = 0
	size = 15

	def __init__(self, pos_x, pos_y, size):
		self.x = pos_x
		self.y = pos_y
		self.size = size
	def render(self, intensity, value, screen):
		font = pygame.font.SysFont("times", 15,bold=True)				
		col = [intensity, intensity, intensity]
		rect = pygame.draw.rect(screen, col, (self.x, self.y, self.size, self.size), 0)
		text = font.render(str(value),1, (255,0,0))
		screen.blit(text,rect,special_flags=0)	



#display some stats on screen
def renderStats(mean_frame_temp, frame_std_dev, high_frame_temp, low_frame_temp, ambient_temp,screen):
	#format all the floats	
	mean_frame_temp_str = str.ljust("Mean temp of this frame",48) + " = %.2f" %mean_frame_temp
	frame_std_dev_str = str.ljust("Std Deviation of this frame",48) + " = %.2f" % frame_std_dev
	high_frame_temp_str = str.ljust("High temp of this frame",48) + " = %.1f" % high_frame_temp
	low_frame_temp_str = str.ljust("Low temp of this frame",48) + " = %.1f" % low_frame_temp	
	ambient_temp_str = str.ljust("Ambient temp of this frame",46) + " = %.1f" %  ambient_temp	
	#date_str = str.ljust("Date of this frame",52) + " = %s" % date

	myfont = pygame.font.SysFont("times", 18)
	CELL_SIZE = 50
	row = 4
	
	#generate and display all labels
	mean_frame_temp_label = myfont.render(mean_frame_temp_str, 1, (255,255,255))
	screen.blit(mean_frame_temp_label, (0,(CELL_SIZE * row)))

	frame_std_dev_label = myfont.render(frame_std_dev_str, 1, (255,255,255))
	screen.blit(frame_std_dev_label, (0,(CELL_SIZE * (row+1))))

	high_frame_temp_label = myfont.render(high_frame_temp_str, 1, (255,255,255))
	screen.blit(high_frame_temp_label, (0,(CELL_SIZE * (row+2))))

	low_frame_temp_label = myfont.render(low_frame_temp_str, 1, (255,255,255))
	screen.blit(low_frame_temp_label, (0,(CELL_SIZE * (row+3))))

	ambient_temp_label = myfont.render(ambient_temp_str, 1, (255,255,255))
	screen.blit(ambient_temp_label, (0,(CELL_SIZE * (row+4))))

	date_label = myfont.render(date_str,1,(255,255,255))
	screen.blit(date_label, (0,(CELL_SIZE * (row+5))))






csv_file_directory = "/home/viki/Documents/IRVideo/IR_CSV_Files/"	
IR_images_directory = "/home/viki/Videos/IRVideo/imgtemp/"
if len(sys.argv) >= 2:
	save_file_boolean = sys.argv[1]
else: 
	save_file_boolean = "False"

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
	cell_size = 50
	
	#initialize pygame display 
	#grid[i].render(data_vector[(i % row_Effective) * column_Effective + (i / 	row_Effective)])				
	pygame.init()
	white = [255,255,255]
	size = [cell_size*column_Effective, cell_size*(row_Effective+5)]
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
		grid[i].render(127.6, 0,screen)
	
	pygame.display.flip()
	try:
		#clean out buffers
		ser.flushInput()
		ser.flushOutput()
			
		#make sure we read both the ambient and the data each cycle
		read_amb = False
		read_dat = False
		
		current_time = strftime("%m-%d-%H:%M:%S",localtime())	
		csvFileName = current_time
		last_frame_date = ''
		counter = 1
		while True:
			response = ser.readline()
			current_time = strftime("%m-%d-%H:%M:%S",localtime())			

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
			 		#compute some stats						
					mean_frame_temp = np.mean(data_vector)
					frame_std_dev = np.std(data_vector)
					high_frame_temp = max(data_vector)
					low_frame_temp = min(data_vector)
					ambient_temp = ambient

					if current_time == last_frame_date:
						image_name = current_time + "(" + str(counter) + ")"
						counter += 1
					else:
						image_name = current_time
						counter = 1


					renderStats(mean_frame_temp, frame_std_dev, high_frame_temp, low_frame_temp, ambient_temp,screen)

					#save image .csv file and image frame if given "-s" cmd line arg
					if save_file_boolean == "-s":
						#write in this row of the .csv file			
						csvFile = open(csv_file_directory+csvFileName+".csv", 'a+')
						ambientTemp = str(ambient)				
						csvFile.write(ambientTemp)   #start with ambient temp
						csvFile.write(',')
						for i in range(0, len(data_vector)):
							value = str(data_vector[i])					
							csvFile.write(value)
							csvFile.write(',')
						csvFile.write(current_time) #time stamp
						csvFile.write(',')
						csvFile.write('\n') 		   #we're done with this row
						csvFile.close()

					data_vector_copy = list(data_vector) #preserve original temp values					

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
						grid[i].render(data_vector[i], data_vector_copy[i], screen)
	
					pygame.image.save(pygame.display.get_surface(), IR_images_directory+image_name+'.jpeg')  
					pygame.display.flip()
					read_amb = False
					read_dat = False 
					last_frame_date = current_time
				
	except Exception, e1:
		print "Error in communication with port: " + str(e1)
  		traceback.print_exc(file=sys.stdout)  #print stack trace

else:
	print "Cannot open serial port"	


