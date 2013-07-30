import cv2
import numpy
import math
import glob
import os

#get a list with all file names
videoFiles = glob.glob("/media/Camera trap/webcam/colorOut/*")
videoFilesIndex = 0

for i in range(0,len(videoFiles)):
	capture = cv2.VideoCapture(videoFiles[videoFilesIndex])
	rval,firstFrame = capture.read()

	while rval != True:
		print "Couldn't read first frame for background. Trying next frame"	
		rval, firstFrame = capture.read()


	sum_back_minus_frame = 0
	frameNumber = 0
	while(True):
		rval, frame = capture.read()
		if rval:	
			backGround_minus_frame = cv2.absdiff(firstFrame, frame)
			sum_back_minus_frame += numpy.sum(backGround_minus_frame)
		else:
			#print ".read() returned false on frameNumber",frameNumber
			break
	frameNumber += 1	

	#print the file name and it's score
	head, tail = os.path.split(videoFiles[videoFilesIndex])
	print tail, sum_back_minus_frame
	videoFilesIndex += 1

