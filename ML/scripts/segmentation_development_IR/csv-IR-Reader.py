#read IR frames from a .csv file.  Compute various metrics on them
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


fileName = sys.argv[1]
stdDev_Average_allFrames = 0
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
			stdDev = np.std(IRValues)			
			maxTemp = max(IRValues)					
			minTemp = min(IRValues)
			tempRange = maxTemp - minTemp
			
			stdDev_Average_allFrames += stdDev

			print "Frame",numFrames,"has Std Dev =",stdDev

except:
	traceback.print_exc(file=sys.stdout)			
	print "I/O Error.  Exiting..."
	exit()	


print "Average Std Dev of all Frames =",((stdDev_Average_allFrames + 0.) / (numFrames + 0.))
