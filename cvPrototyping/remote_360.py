
# Problems:
# chromaticity image is too dense and changes are too subtle for 
# default BGMOG settings

import cv2
import numpy as np
import CameraTrapCV as CTCV

MIN_BLOB_SIZE = 10

if __name__ == "__main__":
    ctcv = CTCV.CameraTrapCV()

    cv2.namedWindow("OG")
    cv2.namedWindow("Out")

    video_in = cv2.VideoCapture(0)
    got_frame, frame = video_in.read()

    while (got_frame == True):
        # Get ROI

        cv2.imshow("OG", frame)

        # Threshold with the background subtractor
        img_thresh = ctcv.bg_subtractor.apply(frame, None, 0.05)

        #img_thresh = cv2.medianBlur(img_thresh, 5)
        if np.count_nonzero(img_thresh) > 5:

            # Get the largest contour
            max_index = ctcv.getLargestContourIndex(img_thresh)

            # Make sure it's big enough
            if cv2.contourArea(ctcv.contours[max_index]) >= MIN_BLOB_SIZE:
                img_out = np.zeros(img_thresh.shape).astype(np.uint8)
                cv2.drawContours(img_out, ctcv.contours, max_index,
                                              (255, 255, 255), -1)
                #print "img_out.shape = " + str(img_out.shape)

                cv2.imshow("Out", img_out)
                x_pos, y_pos = ctcv.getCentroid(ctcv.contours[max_index])

                # Find the angle to turn to
                r, theta = ctcv.getPolar(x_pos, y_pos, img_thresh.shape[1])

                # Kalman filter for smoothness
                # Handle no activity timeout

        else:
            cv2.imshow("Out", np.zeros(frame.shape).astype(np.uint8))
        # Hyphy
        cv2.waitKey(1)
        got_frame, frame = video_in.read()

