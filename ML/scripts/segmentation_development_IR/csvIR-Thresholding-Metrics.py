#take in labeled data from a csv file: ambient_t,temp_1,...,temp_64,blobCount
#run metrics on different thresholding strategies
#only command line arg is the .csv file name

import numpy as np
from numpy import array
#import scipy
import fnmatch
import sys
import csv
import Image
import pygame
import pylab        
pylab.close()
import Pycluster as pc
import traceback
import scipy.stats
import random
import time
import networkx as nx
import math


#calculate optimal threshold...from "A novel statistical image thresholding method"
def J_Alpha_T(t,data):
	class1 = []
	class2 = []
	for i in range(0,len(data)):
		if data[i] < t:
			class1.append(data[i])
		else: 
			class2.append(data[i])
	class1_std_dev = np.std(class1)
	class2_std_dev = np.std(class2)
	J_alpha_t = (.5 * (class1_std_dev**2 + class2_std_dev**2)) + (.5*(class1_std_dev * class2_std_dev))
	return J_alpha_t


#threshold out some values by class and sum Variance 
#method described in "A novel statistical image thresholding method"
def filterGrid_C_S_Vairance(weights):	
	t = 0
	t_opt = 0
	MJ = sys.float_info.max
	for i in range(0,1000):
		J_alpha_t = J_Alpha_T(t, weights)
		if J_alpha_t < MJ:
			MJ = J_alpha_t
			t_opt = t
			t += 1
		else:
			t += 1

	for i in range(0, len(weights)):
		if weights[i] < t_opt:
			weights[i] = 0. #any point not meeting our threshold will have 0 weight
	return weights



#calculate optimal threshold, from "Minimum Standard Deviation Difference-Based
#Thresholding"
def JT(t,data):
	class1 = []
	class2 = []
	for i in range(0,len(data)):
		if data[i] < t:
			class1.append(data[i])
		else: 
			class2.append(data[i])
	class1_std_dev = np.std(class1)
	class2_std_dev = np.std(class2)
	Jt = math.fabs((class1_std_dev - class2_std_dev))
	return Jt


#threshold out some values based on "Minimum Standard Deviation Difference-Based
#Thresholding"
def filterGrid_min_SD(weights):	
	t = 0
	t_opt = 0
	MJ = sys.float_info.max
	for i in range(0,1000):
		Jt = JT(t, weights)
		if Jt < MJ:
			MJ = Jt
			t_opt = t
			t += 1
		else:
			t += 1

	for i in range(0, len(weights)):
		if weights[i] < t_opt:
			weights[i] = 0. #any point not meeting our threshold will have 0 weight

	return weights



#threshold out some values...just throw out values w/ pos z-scores
def filterGrid_pos_zscores(zscores, weights,):	
	for i in range(0,len(zscores)):
		if zscores[i] <= 0:		
			weights[i] = 0. #any point not meeting our threshold will have 0 weight
	return weights


#return the connected components based on the 64 temp values
#which were previously thresholded (a cell has value 0 if it
#was thresholded out
def connectedComponents(data):
	#add any weight/heat that's passed threshold to our graph
	G = nx.Graph()
	G.clear()
	for i in range(0, len(data)):
		if data[i] > 0:
			G.add_node(i, weight=data[i]) #add nodes by index w/ weight attribute too
			neighbors = returnNeighbors(i, 4, 16)
							
			#now, add edges							
			for j in range(0, len(neighbors)): 
				if data[neighbors[j]] > 0:
					G.add_node(neighbors[j], weight=data[neighbors[j]])
					G.add_edge(i,neighbors[j])

	#find the connected components
	ourConnectedComponents = nx.connected_components(G)
	return ourConnectedComponents

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


fileName = sys.argv[1]
class_sum_variance_data = []
class_sum_variance_error = 0
class_sum_variance_offBy = 0
class_sum_variance_offBy_ignore_size_1 = 0
min_std_dev_data = []
min_std_dev_error = 0
min_std_dev_offBy = 0
min_std_dev_offBy_ignore_size_1 = 0
pos_z_score_data = []
pos_z_score_error = 0
pos_z_score_offBy = 0
pos_z_score_offBy_ignore_size_1 = 0
numFrames = 0

try:
	with open(str(fileName), 'rb') as csvfile:
		reader = csv.reader(csvfile)
		for row in reader: #look at every row of .csv
			numFrames += 1
			IRValues = []
			for i in range(1,65): #[0] is ambient temp.  [65] is blob count
				 	IRValues.append(float(row[i]))

			zscores = scipy.stats.zscore(IRValues)	
			maxTemp = max(IRValues)					
			minTemp = min(IRValues)
			tempRange = maxTemp - minTemp

			#make a copy of the raw data for each of our different techniques
			class_sum_variance_data = list(IRValues)	
			min_std_dev_data = list(IRValues)
			pos_z_score_data = list(IRValues)
			
			#threshold based on different techniques
			class_sum_variance_data = filterGrid_C_S_Vairance(class_sum_variance_data)
			min_std_dev_data = filterGrid_min_SD(min_std_dev_data)
			pos_z_score_data = filterGrid_pos_zscores(zscores,pos_z_score_data)

			#find connected components
			class_sum_connected_components = connectedComponents(class_sum_variance_data)
			min_std_dev_connected_components = connectedComponents(min_std_dev_data)
			pos_z_score_connected_components = connectedComponents(pos_z_score_data)
			
			#determine errors
			if len(class_sum_connected_components) != int(row[65]): #row[65] = blob count
				class_sum_variance_error += 1
				offByCount = math.fabs( (len(class_sum_connected_components)  - int(row[65])))  
				class_sum_variance_offBy += offByCount #how many blobs were we off by

				#throw out size 1 CCs when computing the following error metric
				size1Count = 0				
				for i in range(0, len(class_sum_connected_components)):
					if len(class_sum_connected_components[i]) == 1:
						size1Count += 1
				#only count CCs w/ size > 1				
				adjustedCCs = math.fabs( (len(class_sum_connected_components) - size1Count) - int(row[65]))
				class_sum_variance_offBy_ignore_size_1 += adjustedCCs

			if len(min_std_dev_connected_components) != int(row[65]): #row[65] = blob count
				min_std_dev_error += 1
				offByCount = math.fabs( (len(min_std_dev_connected_components)  - int(row[65]))) 
				min_std_dev_offBy += offByCount #how many blobs were we off by

				#throw out size 1 CCs when computing the following error metric
				size1Count = 0				
				for i in range(0, len(min_std_dev_connected_components)):
					if len(min_std_dev_connected_components[i]) == 1:
						size1Count += 1
				#only count CCs w/ size > 1				
				adjustedCCs = math.fabs( (len(min_std_dev_connected_components) - size1Count) - int(row[65]))
				min_std_dev_offBy_ignore_size_1 += adjustedCCs


			if len(pos_z_score_connected_components) != int(row[65]): #row[65] = blob count
				pos_z_score_error += 1
				offByCount = math.fabs( (len(pos_z_score_connected_components)  - int(row[65])))  
				pos_z_score_offBy += offByCount #how many blobs were we off by
				
				#throw out size 1 CCs when computing the following error metric
				size1Count = 0				
				for i in range(0, len(pos_z_score_connected_components)):
					if len(pos_z_score_connected_components[i]) == 1:
						size1Count += 1
				#only count CCs w/ size > 1				
				adjustedCCs = math.fabs( (len(pos_z_score_connected_components) - size1Count) - int(row[65]))
				pos_z_score_offBy_ignore_size_1 += adjustedCCs


except:
	traceback.print_exc(file=sys.stdout)			
	print "I/O Error.  Exiting..."
	exit()	

print ""

print "Class Sum Variance Error = ",(class_sum_variance_error + 0.) / (numFrames + 0.)
print "Class Sum Variance missed and/or extra blobs = ",class_sum_variance_offBy
print "Class Sum Variance missed and/or extra blobs W/0 size 1 CCs = ",class_sum_variance_offBy_ignore_size_1
print ""

print "Min Std Dev Error = ",(min_std_dev_error + 0.) / (numFrames + 0.)
print "Min Std Dev missed and/or extra blobs = ",min_std_dev_offBy
print "Min Std Dev missed and/or extra blobs W/0 size 1 CCs = ",min_std_dev_offBy_ignore_size_1
print ""


print "Positive Z Score Error = ",(pos_z_score_error + 0.) / (numFrames + 0.)
print "Positive Z Score missed and/or extra blobs = ",pos_z_score_offBy
print "Positive Z Score missed and/or extra blobs W/0 size 1 CCs = ",pos_z_score_offBy_ignore_size_1
print ""





