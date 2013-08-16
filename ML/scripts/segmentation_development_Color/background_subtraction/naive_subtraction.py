# Compare naive background subtraction (just update the background
# by new_background = current_background + alpha*(current_background - new_frame)\
# with MOG background subtraction.  
# Grayscales the Video Capture you specify.  

import cv2
import numpy
import time

# update the background using an alpha parameter.
# new_background = current_background + alpha*(current_background - new_frame)
def update_background(new_frame, current_background, alpha):
    current_background = current_background.astype(float)
    new_frame = new_frame.astype(float)
    difference = numpy.subtract(new_frame, current_background) 
    background = numpy.multiply(difference, alpha)
    background = numpy.add(current_background, background)  
    background = numpy.round(background) 
    background = numpy.uint8(background)
    return background


c = cv2.VideoCapture("/home/sealab/Downloads/August 7th 2013/file_1_low_resolution.mov")
background_mog = cv2.BackgroundSubtractorMOG()

frame_count = 0
while(1):
    got_a_frame,frame = c.read()
    if got_a_frame == True:
        #first frame initialized to background        
        if frame_count == 0:
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)        
            background = frame      

        else:  
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            difference = cv2.absdiff(frame, background)
           
            #grayscale differences greater than 30 are determined to be foreground
            for i in range(0, len(difference)):
                for j in range(0, len(difference[i])):
                    if difference[i][j] > 30:
                        difference[i][j] = 255
                    else:
                        difference[i][j] = 0
          
            foreground_mask = background_mog.apply(frame, None, 0.025)    
            cv2.imshow('Raw Camera',frame)
            cv2.imshow('Naive Foreground',difference)
            cv2.imshow('MOG: Fore', foreground_mask)        
            background = update_background(frame, background, .025)
            
    else:
        break	
    print frame_count
    frame_count += 1    
    if cv2.waitKey(5)==27:
        break


print "Broke out of the loop."
cv2.destroyAllWindows()
