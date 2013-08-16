# mixture of gaussians visualizer.
# grayscale frames, then run MOG background subtraction.

import cv2
import numpy
import time



c = cv2.VideoCapture("/home/sealab/Downloads/attachments/file_3_low_resolution.mov")
background_mog = cv2.BackgroundSubtractorMOG()
while(1):
    got_a_frame,frame = c.read()
    if got_a_frame == True:
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)	
        foreground_mask = background_mog.apply(frame, None, 0.01)
        cv2.imshow('Grayscaled Camera',frame)
        cv2.imshow('MOG: Foreground Mask',foreground_mask)
    else:
        break	
    if cv2.waitKey(5)==27:
        break

cv2.destroyAllWindows()
