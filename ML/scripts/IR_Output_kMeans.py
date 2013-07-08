import numpy as np
from numpy import array
#import scipy
import fnmatch
import sys
import serial
import Image
import pygame
import pylab        
pylab.close()
import Pycluster as pc
import traceback
import scipy.stats




class Cell:
	x = 0
	y = 0
	size = 15
	intensity = 0;
	def __init__(self, pos_x, pos_y, size):
		self.x = pos_x
		self.y = pos_y
		self.size = size
	def render(self, intensity):
		self.intensity = intensity
		col = [intensity, intensity, intensity]
		pygame.draw.rect(screen, col, (self.x, self.y, self.size, self.size), 0)
	def centerRender(self): 
		#draw centers as red		
		pygame.draw.rect(screen, [255,0,0], (self.x, self.y, self.size, self.size), 0)
	def hotSpotRender(self):
		#draw hottest spot as yellow		
		pygame.draw.rect(screen, [255,255,0], (self.x, self.y, self.size, self.size), 0)




#returns the coordinates of our grid:
# A B C
# D E F
# G H I
#where G has coordinate (0,0)
#index the returned array such that A is at [0], D at [1], G at [2], B at [3]... 
def makeGrid(rows, columns):
	data = np.array([(0,(rows-1))]) #having issues with compilation, hardcode the first point		
	columnCount = 0	
	while columnCount < columns:
		rowCount = rows - 1		
		while rowCount >= 0:	
			if (columnCount != 0 or rowCount != (rows -1)): #ignore the very first pass, we hardcoded it			
				nextGridCoordinate = np.array([(columnCount,rowCount)])
				data = np.concatenate((data,nextGridCoordinate), axis=0)
			rowCount -= 1
		columnCount += 1 	
	return data	


#we'll weight each point by adding "heat value" number of points
#so, if (1,1) has heat value 10, we'll have 10 (1,1)s in our data set
# A B C
# D E F
# G H I
#IR returns heat readings in this order: A, D, G, B, E, H...
#we'll just multiply the weights by 10 for now to be sure they're whole numbers.... :(
def addWeighting(data, heat):
	for i in range(0, len(data)):
		gridIndex = getGridIndexOfCenter(data[i])		
		numAdditionalPoints = int((heat[gridIndex])*10)		
		for j in range(0, numAdditionalPoints):  #add that points heat*10 additional points
			additionalDataPoint = ([data[i]])
			data = np.vstack((data,additionalDataPoint))
	return data


#k-means will return floating point centers--eg (1.3,2.123)
#We want to determine what cell this corresponds to ( (1,2) for the above),
#and what index that cell has in the grid array
def getGridIndexOfCenter(center):
	#we use the lower left corner of a given cell for coordinates, so floor(x) and floor(y) gives the appropriate (x,y) coord
	x = int(center[0])  
	y = int(center[1])
	
	#grid is indexed as described in comment on top of makeGrid, so do some math to determine the appropriate index given the coordinate
	index = (3-y) + (4*x) 
	return index


#only look at points w/ a positive z-score
def filterGrid(zscores, standardDeviation, ourGrid):
	positive_ZScore_Count = 0
	for i in range(0,len(zscores)):
		if zscores[i] > (.2*standardDeviation):		
			if positive_ZScore_Count == 0:
				newGrid = ([ourGrid[i]])
			else:
				point = ([ourGrid[i]])
				newGrid = np.vstack((newGrid,point))
			positive_ZScore_Count += 1
	
	if positive_ZScore_Count == 0:  #no points were past our threshold
		raise NameError("No-Values-Above-Threshold")
		print "No Values Above Threshold"
	return newGrid


#color the pygame window black
def colorWindowBlack():
	for i in range(len(data_vector)):
		grid[i].render(0)
	pygame.display.flip()


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




#serial port magic
try: 
	ser.open()        

except Exception, e:
	print "Error opening port :( " + str(e)
	exit()

if ser.isOpen():
	cell_size = 65
	
	#initialize pygame display 
	pygame.init()
	white = [255,255,255]
	size = [cell_size*column_Effective, cell_size*row_Effective]
	screen = pygame.display.set_mode(size)
	screen.fill(white);
	grid = [];
	filterGrid
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
		
		print "Started loop"
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
	
				#transform input string into float array
				try:
					data_list = data.split()
					data_vector = map(float, data_list) 
				except:
					print "Could not convert IR sensor data to float values"
					continue
				#be sure we got all 64 values
				if len(data_vector) == 64:
					#create our 4X16 grid
					ourGrid = makeGrid(4,16)

					zscores = scipy.stats.zscore(data_vector)
					standardDeviation = np.std(data_vector)

					#filter out points by z-score...
					try:					
						filteredGrid = filterGrid(zscores, standardDeviation,ourGrid)			
					except:					
						colorWindowBlack()
						continue						


					#weight our grid by the heat values we just received
					filteredGrid = addWeighting(filteredGrid,data_vector)

					#run kmeans
					k = 1
					clusterid, error, nfound = pc.kcluster(filteredGrid, nclusters=k, mask=None, weight=None, transpose=0, npass=1, method = 'a', dist = 'e', initialid=None)

					centers, _ = pc.clustercentroids(filteredGrid, clusterid=clusterid)
	
					maxTemp = max(data_vector)					
					minTemp = min(data_vector)
					tempRange = maxTemp - minTemp

					#color all cells
					for i in range(len(data_vector)):
						#we are ignoring anything with a negative z-score						
						if zscores[i] < 0:
							grid[i].render(0)
						else:
							intensity = data_vector[i] - minTemp						
							colorIntensity = intensity / tempRange					
							color = colorIntensity * 255
							if color == 255:
								grid[i].hotSpotRender()  #color hottest spot yellow 
							else:
								grid[i].render(color) 
					
				 
					#color the centers red
					for i in range(len(centers)):
						centerIndex = getGridIndexOfCenter(centers[i])
						grid[centerIndex].centerRender() 

					
					pygame.display.flip()

					read_amb = False
					read_dat = False 
			
	except Exception, e1:
		print "Error in communication with port: " + str(e1)
    	traceback.print_exc(file=sys.stdout)

else:
	print "Cannot open serial port"	



