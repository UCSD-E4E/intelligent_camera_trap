import cv2
import numpy
import time



c = cv2.VideoCapture("/home/sealab/Downloads/Video Jul 30, 5 43 59 PM.mov")
background_mog = cv2.BackgroundSubtractorMOG()

while(1):
	got_a_frame,frame = c.read()
	if got_a_frame == True:
		frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)	
		foreground_mask = background_mog.apply(frame, None, 0.01)
		cv2.imshow('Raw Camera',frame)
		cv2.imshow('Foreground Mask',foreground_mask)
	else:
		break	
	if cv2.waitKey(5)==27:
		break
	time.sleep(.05)
cv2.destroyAllWindows()
