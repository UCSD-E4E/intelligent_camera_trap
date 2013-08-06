/*
 * ADJUST FRAME SIZE IN BOTH FRAME.HPP, AND VIBE.HPP.
 */




#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <boost/multi_array.hpp>
#include "ViBe.hpp"
#include "Frame.hpp"




int main(int argc, char *argv[]) {
    cv::Mat frame;
    cv::Mat gray;
    cv::Mat fore;
	cv::VideoCapture cap("/home/sealab/Downloads/outdoor_smallest.mov");
	ViBe* bg_vibe = new ViBe(2, 40, 5);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, VIDEO_WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, VIDEO_HEIGHT);
	fore = cv::Mat(VIDEO_HEIGHT, VIDEO_WIDTH, CV_8U, cv::Scalar(0));
	int frame_count = 0;
    Frame currentFrame;
    Frame output;

	for (;;) {
		cap >> frame;
		cv::cvtColor(frame, gray, CV_RGB2GRAY);
		cv::imshow("original", gray);
		int h, w;
		for (w = 0; w < gray.cols; w++) {
			for (h = 0; h < gray.rows; h++) {
                currentFrame.picture[w][h] = gray.at<char>(h,w);
            }			
		}
		if (frame_count == 0){
			bg_vibe->initBg(currentFrame);
		}
		else{
			bg_vibe->updateModel(currentFrame, output);
            int x, y;
            for (y = 0; y < gray.rows; ++y)
            {
                for( x = 0; x < gray.cols; ++x)
                {
                    fore.at<char>(y,x) = output.picture[x][y];
                }
            } 
			//fore = cv::Mat(VIDEO_WIDTH, VIDEO_HEIGHT, CV_8U, &foreground);
			//fore2 = cv::Mat(VIDEO_HEIGHT, VIDEO_WIDTH, CV_8U, &foreground);
			//rotate(fore, 270, fore2);
			cv::imshow("mask", fore);
			//cv::imshow("original", frame);
		}
		if (cv::waitKey(30) >= 0)
			break;
        frame_count++;
	}
	return 0;
}
