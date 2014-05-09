# CameraTrapCV.py
# -Computer vision functions for the Intelligent Camera Trap!

# problems:
#
#

import cv2
import numpy as np
import math

class CameraTrapCV():
    def __init__(self):
        self.PI = 3.14159
        self.bg_subtractor = cv2.BackgroundSubtractorMOG(history=200, 
            nmixtures=5, backgroundRatio=0.7, noiseSigma=0)

        self.kalman = cv2.KalmanFilter()

    # returns the center of a contour
    def getCentroid(self, contour_in):
        moments = cv2.moments(contour_in)
        x = int(moments['m10'] / moments['m00'])
        y = int(moments['m01'] / moments['m00'])
        return [x, y]

    # converts (x, y) to (r, theta)
    def getPolar(self, x, y, frame_w):
        r = (x ** 2 + y ** 2) ** 1/2
        theta = math.atan2(y, (x-frame_w)/2) * 180. / self.PI
        return [r, theta]


    # converts an image to a chromaticity image with normalized RGB values
    # ranging between 0 and 255. This color space ignores shadows!
    def getChromaticity(self, img_in):
        img_float = img_in.astype(np.float32)
        img_chroma = np.zeros(img_in.shape)
        img_norm = np.sum(img_float, 2)
        for i in range(0, 3):
            img_chroma[:,:,i] = np.divide(img_float[:,:,i], img_norm)
        return (img_chroma * 255).astype(np.uint8)

    def sphericalAngleTransform(self, img_chroma):
        img_sphere = np.zeros(img_chroma.shape)
        img_sphere[:,:,0] = np.arctan2(img_chroma[:,:,2], img_chroma[:,:,1])
        rg_mag = np.sqrt(np.power(img_chroma[:,:,2], 2) +
            np.power(img_chroma[:,:,1], 2))
        img_sphere[:,:,1] = np.arctan2(img_chroma[:,:,0], rg_mag)
        img_sphere[:,:,0] = np.add(img_sphere[:,:,0], self.PI/2.)
        img_sphere[:,:,1] = np.add(img_sphere[:,:,1], self.PI/2.)
        img_sphere = img_sphere * 255./self.PI
        return img_sphere.astype(np.uint8)


    def getLargestContourIndex(self, img_thresh):
        self.contours, hierarchy = cv2.findContours(img_thresh, cv2.RETR_TREE,
            cv2.CHAIN_APPROX_SIMPLE)

        # totally not from stack overflow
        areas = [cv2.contourArea(c) for c in self.contours]
        i_max  = np.argmax(areas)
        return i_max

    def getTopHalfImg(img_in):
      img_out = img_in[0:img_in.shape[0] / 2, 0:img_in.shape[1]/2]
