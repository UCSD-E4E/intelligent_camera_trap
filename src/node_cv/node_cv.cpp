#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cxcore.h>
#include <opencv/cvblob.h>

#include "CamTrap_Viper/CvService.h"
#include "cv_localizer.h"

using namespace cvb;
using namespace std;


int main(int argc, char **argv)
{
	const int FLIR_FRAME_WIDTH  = 480;
   const int FLIR_FRAME_HEIGHT = 320;
   const int FLIR_FOV_X = 36;
   const int FLIR_FOV_Y = 27;

	ros::init(argc, argv, "cv_service");
	ros::NodeHandle n;

	CVLocalizer object_tracker(0, 0, FLIR_FRAME_WIDTH, FLIR_FRAME_HEIGHT, FLIR_FOV_X, FLIR_FOV_Y);
	object_tracker.setTimestamp(0);

	ros::ServiceServer service = n.advertiseService("cv_service", &CVLocalizer::newCoords, &object_tracker);

	//Initialize Cameras:	
	// set the norm to NTSC for FLIR - and input to 1 for capture device  
	system("v4l2-ctl -s NTSC -i 1");   //if FLIR is video0
	//system("v4l2-ctl -d /dev/video1 -s NTSC -i 1"); if FLIR is video1
	
	/* Initialize the IRCam */
	CvCapture *capture = cvCreateCameraCapture(0);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, FLIR_FRAME_WIDTH);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, FLIR_FRAME_HEIGHT);
  
	/* Always check if the program can find a device */
	if (!capture)
	{
		printf("error 1");
		ROS_ERROR("Can not open flir");
		return -1;
	}

	/* creating display window */
	//use only for testing
	cvNamedWindow( "FLIR",CV_WINDOW_AUTOSIZE);

	/* Create required images once */
	IplImage *img = cvQueryFrame(capture);
	IplImage  *gray_img = cvCreateImage(cvSize(FLIR_FRAME_WIDTH, FLIR_FRAME_HEIGHT), img->depth, 1);
 	IplImage  *thres_img = cvCreateImage(cvSize(FLIR_FRAME_WIDTH, FLIR_FRAME_HEIGHT), img->depth, 1);
	IplImage  *label_img = cvCreateImage(cvSize(FLIR_FRAME_WIDTH, FLIR_FRAME_HEIGHT), IPL_DEPTH_LABEL, 1);

	/* Time init */
	time_t rawTime = time (NULL);
	tm *pTime = gmtime(&rawTime);
	
	/* initialize video writer */
	CvVideoWriter *flirWriter;
	char fname[100];
	sprintf(fname, "%s%d%d%d%s", "/home/viki/Videos/", pTime->tm_hour, pTime->tm_min, pTime->tm_sec, ".avi");
	flirWriter = cvCreateVideoWriter(fname, CV_FOURCC('D','I','V','X'), 17, cvSize(w,h), 1);
	
	/* main loop */
	int T;
	double moment10;
	double moment01;
	double area;
	int duration_sec = 60;
	long int timecnt = time(&rawTime) + duration_sec;
  	static int posX;
  	static int posY;
	CvBlobs blobs;
	CvLabel bigstblob;

while (ros::ok())
{
	/* Obtain a frame from the device */
	img = cvQueryFrame(capture);

	/* Always check if the device returns a frame */
	if( !img )
	{
		ROS_ERROR("Error retrieving FLIR frame\n");
		return -1;
	}

	/* remove the watermark */
	cvSetImageROI(img, cvRect((FLIR_FRAME_WIDTH*367)/480,(FLIR_FRAME_HEIGHT*14)/320,(FLIR_FRAME_WIDTH*90)/480,(FLIR_FRAME_HEIGHT*28)/320));
	cvZero(img);
	cvResetImageROI(img);

	/* Time get */
	rawTime = time (NULL);

	/* reset video writer for every X second */
	if (timecnt <= time(&rawTime))
	{
		cvReleaseVideoWriter(&flirWriter);
		pTime = gmtime(&rawTime);
		sprintf(fname, "%s%d%d%d%s", "/home/viki/Videos/", pTime->tm_hour, pTime->tm_min, pTime->tm_sec, ".avi");
		flirWriter = cvCreateVideoWriter(fname, CV_FOURCC('D','I','V','X'), 17, cvSize(w,h), 1);
		timecnt = time(&rawTime) + duration_sec;
	}

	/* Write images to file */
	cvWriteFrame(flirWriter, img);
		
	// Flipping the img if needed with motor node
	//	cvFlip(frame_img, frame_img, 1);

  	/* Convert image from Color to grayscale */
  	cvCvtColor(img, gray_img, CV_RGB2GRAY); 

	/* Filter by applying threshold */
	T = 1.7*cvMean(gray_img);
	cvThreshold(gray_img, thres_img, T, 255, CV_THRESH_BINARY);
	cvSmooth(thres_img, thres_img, CV_MEDIAN, 7, 7);//optional smoothing
	
	/* Find Blobs that are White, Hence 'uchar backgroundColor = 0' (Black) */
	unsigned int result = cvLabel(thres_img, label_img, blobs);

	/* Filter vertical blobs ( between 1 & -1 rad)*/
	CvBlobs::iterator ita=blobs.begin();
	while(ita!=blobs.end())
	{
		CvBlob *blob=(*ita).second;
		if ((cvAngle(blob)<-1.0)||(cvAngle(blob)>1.0))
		{
			cvReleaseBlob(blob);
			CvBlobs::iterator tmp=ita;
			++ita;
			blobs.erase(tmp);
		}
		else
			++ita;
	}	

	/* Filter by area (less than biggest blob)*/
	bigstblob = cvGreaterBlob(blobs);
	cvFilterByLabel( blobs, bigstblob);

	/* Rendering the blobs */
	cvRenderBlobs(label_img, blobs, img, img);
 
	/* determine the center of blob and it's movement */
	if(bigstblob)
	{
		CvBlobs::const_iterator it=blobs.begin();
		moment10 = it->second->m10;
		moment01 = it->second->m01;
		area = it->second->area;
		posX = (moment10/area)-(FLIR_FRAME_WIDTH/2);
		posY = ((moment01/area)-(FLIR_FRAME_HEIGHT/2));
		//height starts from the top, add a "-" to inverse posY if it's necessary
	}
	else
	{
		posX = 0; 
		posY = 0;
	}

	object_tracker.x = posX;
	object_tracker.y = posY;
	
	/* Publish videos */
	ROS_INFO("\nx = %d, y = %d, Time = %ld", posX, posY, time(&rawTime));
	cvShowImage( "FLIR", img);

	//for use in node if cvShowImage didn't work
	/*	cv::Mat flir (img);
		cv::namedWindow (FLIR, CV_WINDOW_AUTOSIZE);
		cv::imshow ("FLIR", flir);*/

	/* cleaning memory */
	cvWaitKey(1);
	cvZero(img);
	cvZero(gray_img);
	cvZero(thres_img);
	cvZero(label_img);

	}
 	
	/* Clean up memory */
	//since the loop will break by ros::ok this part is not useable
	/*	cvReleaseCapture( &capture );
	cvDestroyAllWindows();*/
	
	return 0;
}


