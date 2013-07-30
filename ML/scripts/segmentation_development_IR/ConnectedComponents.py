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
import networkx as nx



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
	def blueRender(self):
		pygame.draw.rect(screen, [0,0,255], (self.x, self.y, self.size, self.size), 0)
	def blueScaleRender(self, intensity):
		pygame.draw.rect(screen, [0,0,intensity], (self.x, self.y, self.size, self.size), 0)
	def customRender(self, customColor):
		pygame.draw.rect(screen, customColor, (self.x, self.y, self.size, self.size), 0)		

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




#given a particular index in our grid return the 
#indicies of neighboring cells
def returnNeighbors(index, rows, columns):
	neighbors = []	
	if ((index == 0) or ((index % rows) == 0)): #on the top of a column
		neighbors = topOfColumnNeighbors(index,rows,columns)
	elif ((index + 1) % rows) == 0: #on bottom of column
		neighbors = bottomOfColumnNeighbors(index,rows,columns)
	else: #cell somewhere in middle of grid
		neighbors = allPossibleNeighbors(index, rows, columns)
	return neighbors 
					

#all possible neighbors of a particular index in our grid
def allPossibleNeighbors(index, rows, columns):
	possibleNeighbors = []
	possibleNeighbors.append(index - 1)
	possibleNeighbors.append(index + 1)
	possibleNeighbors.append(index - rows)
	possibleNeighbors.append(index + rows)
	possibleNeighbors.append(index - (rows + 1))
	possibleNeighbors.append(index + (rows + 1))
	possibleNeighbors.append(index - (rows - 1))
	possibleNeighbors.append(index + (rows - 1))
	
	#throw out negative and out of bounds indicies
	i = 0
	while i < len(possibleNeighbors):
		if (possibleNeighbors[i] < 0) or (possibleNeighbors[i] >= (rows*columns)):
			possibleNeighbors.pop(i)
		else:
			i += 1
	
	return possibleNeighbors

embedded
#return the possible neighbors of an index that's at the top of a column
def topOfColumnNeighbors(index,rows,columns):
	toBeRemoved = []	
	allNeighbors = allPossibleNeighbors(index,rows,columns)
	for i in range(0, len(allNeighbors)):
		if (allNeighbors[i] == (index - 1)):
			toBeRemoved.append(index - 1)
		if (allNeighbors[i] == (index - (rows + 1))):
			toBeRemoved.append(index - (rows + 1))
		if (allNeighbors[i] == (index + (rows - 1))):
			toBeRemoved.append(index + (rows - 1))
	for i in range(0, len(toBeRemoved)):
		allNeighbors.remove(toBeRemoved[i])	
	return allNeighbors



def bottomOfColumnNeighbors(index,rows,columns):
	toBeRemoved = []
	allNeighbors = allPossibleNeighbors(index,rows,columns)
	for i in range(0, len(allNeighbors)):
		if (allNeighbors[i] == (index + 1)):
			toBeRemoved.append(index + 1)
		if (allNeighbors[i] == (index - (rows - 1))):
			toBeRemoved.append(index - (rows - 1))
		if (allNeighbors[i] == (index + (rows + 1))):
			toBeRemoved.append(index + (rows + 1))
	for i in range(0, len(toBeRemoved)):
		allNeighbors.remove(toBeRemoved[i])
	return allNeighbors




#k-means will return floating point centers--eg (1.3,2.123)
#We want to determine what cell this corresponds to ( (1,2) for the above),
#and what index that cell has in the grid array
def getGridIndexOfCenter(center, rows):
	#we use the lower left corner of a given cell for coordinates, so floor(x) and floor(y) gives the appropriate (x,y) coord
	x = int(center[0])  
	y = int(center[1])
	
	#grid is indexed as described in comment on top of makeGrid, so do some math to determine the appropriate index given the coordinate
	index = ((rows-1)-y) + (rows*x) 
	return index


#threshold out some values
def filterGrid(zscores, weights, ourGrid):	
	#determine average of positive zscores	
	count = 0.
	positiveSum = 0.
	for i in range(0,len(zscores)):
		if zscores[i] > .5:
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
		print "No Values Above Threshold"		
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




#serial port magic
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
kastner@ucsd.edu

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
				except:
					print "Could not convert IR sensor data to float values"
					continue
				
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
						#anything that was assigned 0 weight/heat colorst, we've filtered out					
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
					
					#add any weight/heat that's passed threshold to our graph
					G = nx.Graph()
					G.clear()
					for i in range(0, len(data_vector)):
						if data_vector[i] > 0:
							G.add_node(i, weight=data_vector[i]) #add nodes by index w/ weight attribute too
							neighbors = returnNeighbors(i, 4, 16)
							
							#now, add edges							
							for j in range(0, len(neighbors)): 
								if data_vector[neighbors[j]] > 0:
									G.add_node(neighbors[j], weight=data_vector[neighbors[j]])
									G.add_edge(i,neighbors[j])

					#find the connected components
					connectedComponents = nx.connected_components(G)

					#color the connected components
					differentColors = [[0,255,0],[0,0,255],[0,255,255],[255,0,255], [255,255,0],[0,0,255]]
					differentColorsIndex = 0					
					for i in range(0,len(connectedComponents)):
						for j in range(0,len(connectedComponents[i])):
							nodeIndex = connectedComponents[i][j]
							grid[nodeIndex].customRender(differentColors[differentColorsIndex])
						differentColorsIndex += 1
						if differentColorsIndex >= len(differentColors):
							differentColorsIndex = 0

	
					#find hottest CC, we'll move camera to it's weighted average
					hottest = 0
					hottestIndex = 0
					for i in range(0,len(connectedComponents)):
						heatSum = 0						
						for j in range(0,len(connectedComponents[i])):
							nodeIndex = connectedComponents[i][j]
							heatSum += data_vector[nodeIndex]
						if heatSum > hottest:
							hottest = heatSum
							hottestIndex = i

					#determine weighted average of hottest CC 
					weightedIndicies = 0
					sumOfWeights = 0					
					for i in range(0,len(connectedComponents[hottestIndex])):
						nodeIndex = connectedComponents[hottestIndex][i]	
						weightedIndicies += data_vector[nodeIndex] * nodeIndex
						sumOfWeights += data_vector[nodeIndex]
					indexToTrack = weightedIndicies / sumOfWeights
					indexToTrack = int(round(indexToTrack))
					grid[indexToTrack].customRender([255,255,255])
						

					pygame.display.flip()

					read_amb = False
					read_dat = False 
			
	except Exception, e1:
		print "Error in communication with port: " + str(e1)
    	traceback.print_exc(file=sys.stdout)

else:
	print "Cannot open serial port"	



