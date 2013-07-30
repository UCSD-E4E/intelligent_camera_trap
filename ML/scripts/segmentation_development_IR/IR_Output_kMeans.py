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
import random




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
		col = [intensity, intensity, intens    	traceback.print_exc(file=sys.stdout)ity]
		pygame.draw.rect(screen, col, (self.x, self.y, self.size, self.size), 0)
	def centerRender(self): 
		#draw centers as red		
		pygame.draw.rect(screen, [255,0,0], (self.x, self.y, self.size, self.size), 0)
	def hotSpotRender(self):
		#draw hottest spot as yellow		
		pygame.draw.rect(screen, [255,255,0], (self.x, self.y, self.size, self.size), 0)


def runKMeans(k, data, weights, initialCenters, repeat):
	centerAssignments, linedUpWeights = assignToCenters(data, initialCenters, weights)

	#adjust centers to be the weighted mean of the points that constitute them
	newCenters = adjustCenters(initialCenters, centerAssignments, linedUpWeights)				

	for i in range(0,repeat):
		centerAssignments, linedUpWeights = assignToCenters(data, newCenters, weights)
		newCenters = adjustCenters(newCenters, centerAssignments, linedUpWeights)		
	

	#calculate the value of our cost fcn after running k-means	
	centerAssignments, linedUpWeights = assignToCenters(data,newCenters,weights)
	sumCostFcn = 0	
	for i in range(0,len(centerAssignments)):		
		for j in range(0,len(centerAssignments[i])):
			pointsWeight = linedUpWeights[i][j] + 0.						
			sumCostFcn += weightedDistanceGi    	traceback.print_exc(file=sys.stdout)venWeight(centerAssignments[i][j], newCenters[i], pointsWeight) 
	return newCenters, sumCostFcn	



def assignToCenters(data, centers, weights):
	#list of lists to hold which points go with which centers		
	centerAssignments = [[] for x in xrange(len(centers))]

	#list of lists to hold the weights of points with each center		
	linedUpWeights = [[] for x in xrange(len(centers))]

	for i in range(0,len(data)):
		smallestDist = -1
		closestCenter = 0
		for j in range(0,len(centers)):
			distance = weightedDistance(data[i], centers[j], weights,i)
			if ((distance < smallestDist) or smallestDist == -1):
				smallestDist = distance 
				closestCenter = j
		centerAssignments[j].append(data[i])
		linedUpWeights[j].append(weights[i])
	return centerAssignments, linedUpWeights


def adjustCenters(centers, centerAssignments, linedUpWeights):
	adjustedCenters = np.empty((len(centers),2)) #we have len(centers) # of centers each with an x,y coord
	for i in range(0,len(centerAssignments)):
		sumOfWeights = 0	
		sumOfWeightedX = 0
		sumOfWeightedY = 0		
		for j in range(0,len(centerAssignments[i])):
			pointsWeight = linedUpWeights[i][j] + 0.			
			sumOfWeights += pointsWeight
			sumOfWeightedX += ((centerAssignments[i][j][0] + 0.) * pointsWeight)
			sumOfWeightedY += ((centerAssignments[i][j][1] + 0.) * pointsWeight)
		if sumOfWeights == 0: #no points closest to this particular center
			newX = centers[i][0] + 0.
			newY = centers[i][1] + 0.
		else:		
			newX = float(sumOfWeightedX / sumOfWeights)
			newY = float(sumOfWeightedY / sumOfWeights)
		adjustedCenters[i][0] = newX
		adjustedCenters[i][1] = newY
	return adjustedCenters
			
				


def weightedDistance(point, center, weights, index): 
	distance = np.linalg.norm(point - center)
	distance *= weights[index]
	return distance


def weightedDistanceGivenWeight(point, center, weight):
	distance = np.linalg.norm(point - center)
	distance *= weight
	return distance
    	traceback.print_exc(file=sys.stdout)


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


    	traceback.print_exc(file=sys.stdout)

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


#threshold out some values
def filterGrid(zscores, weights, ourGrid):	
	#determine average of positive zscores	
	count = 0.
	positiveSum = 0.
	for i in range(0,len(zscores)):
		if zscores[i] > 0:
			positiveSum += zscores[i]
			count += 1.
	positive_zscore_avg = positiveSum / count

	positive_ZScore_Count = 0	
	for i in range(0,len(zscores)):
		if zscores[i] < .5:		
			weights[i] = 0. #any point not meeting our threshold will have 0 weight
		else:
			positive_ZScore_Count += 1	

	if positive_ZScore_Count == 0:  #no points were past our threshold
		print "No Values Above Threshold"	    	traceback.print_exc(file=sys.stdout)	
		raise NameError("No-Values-Above-Threshold")
	return weights


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


#our initial center
center = [[(7,2)]]

#serial port magic    	traceback.print_exc(file=sys.stdout)
try: 
	ser.open()        

except Exception, e:
	print "Error opening port :( " + s
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
		read_amb = False    	traceback.print_exc(file=sys.stdout)
		read_dat = False

		#on our first run, we'll seed kmeans with a center in about the middle of the grid 
		#(subsequently, we'll seed with the most recently calculated center)
		center = array([(7,2)])
		print "Started loop"
		while True:	
			response = ser.readline()
			#add len(responinitalCentersse) <= 16 to address bug where sometimes we read lines like this:
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
				except:    	traceback.print_exc(file=sys.stdout)
					print "Could not convert IR sensor data to float values"
					continue
				print data_vector				

				#be sure we got all 64 values
				if len(data_vector) == 64:
					#create our 4X16 grid
					ourGrid = makeGrid(4,16)


					#get some info about our heat values
					zscores = scipy.stats.zscore(data_vector)	
					maxTemp = max(data_vector)					
					minTemp = min(data_vector)
					tempRange = maxTemp - minTemp


					#filter out points by z-score...
					try:					
						#anything not meeting threshold will be assinged 0 heat/"weight"					
						data_vector = filterGrid(zscores, data_vector, ourGrid)			
					except:
						colorWindowBlack()
						continue						
					
					#color all cells
					for i in range(len(data_vector)):    	
						#anything that was assigned 0 weight/heat, we've filtered out					
						if data_vector[i] <= 0:
							grid[i].render(0)
						else:
							intensity = data_vector[i] - minTemp						
							colorIntensity = intensity / tempRange					
							color = colorIntensity * 255
							if color == 255:
								grid[i].hotSpotRender()  #color hottest spot yellow 
							else:
								grid[i].render(color) 
					
					"""
					#square all heat/"weight" values to try and amplify effects of hot spots
					for i in range(0,len(data_vector)):
						data_vector[i] = data_vector[i]**2
					"""
				

					k = 1	
					center, costFcnValue = runKMeans(k, ourGrid, data_vector, center, 5)				
				
					#color the centers red
					for i in range(len(center)):
						centerIndex = getGridIndexOfCenter(center[i])
						grid[centerIndex].centerRender() 

					
					pygame.display.flip()

					read_amb = False
					read_dat = False 
			
	except Exception, e1:
		print "Error in communication with port: " + str(e1)
    	traceback.print_exc(file=sys.stdout)

else:
	print "Cannot open serial port"	



