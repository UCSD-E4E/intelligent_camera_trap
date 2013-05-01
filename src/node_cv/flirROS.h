//FLIR Camera OpenCV Library
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <iostream>
#include </opt/ros/fuerte/include/opencv/cv.h>
#include </opt/ros/fuerte/include/opencv/highgui.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include </opt/ros/fuerte/include/opencv/cxcore.h>
#include </usr/local/include/cvblobs/BlobResult.h>
#include </opt/ros/fuerte/include/opencv/cvblob.h>
#include </usr/local/include/cvblobs/BlobOperators.h>
#include "parameters.h"
#include "karp.h"

int PicHeight = 720;
int PicWidth = 960;

void GetDegreesFromCenter(int x, int y,double out[2] )
{

	out[0] = ((double)x-PicWidth/2)/PicWidth/2; //+1 cuz pixel counting starts at 0 but width starts at 1
	out[1] = ((double)y-PicHeight/2)/PicHeight/2;

}

int trackObject()
{

	/* set the norm to NTSC for FLIR - and input to 1 for capture device - */
	system("v4l2-ctl -s NTCS -i 1");
	
	/* Initialize the webcam */
	CvCapture *capture1 = cvCaptureFromCAM(0);
	cvSetCaptureProperty(capture1, CV_CAP_PROP_FRAME_WIDTH, PicWidth);
	cvSetCaptureProperty(capture1, CV_CAP_PROP_FRAME_HEIGHT, PicHeight);
	cvSetCaptureProperty(capture1, CV_CAP_PROP_FPS, 24);	
     
	/* Always check if the program can find a device */
        if (!capture1){
        	ROS_ERROR("Can not open flir");
                return -1;}

        /* Obtain a frame from the device */
	IplImage  *img = cvQueryFrame( capture1 );
	/* Always check if the device returns a frame */
	if( !img )
		{
			ROS_ERROR("Error retrieving FLIR frame\n");
			return -1;
		}

	/* Create required images once */
 	IplImage  *gray_img = cvCreateImage( cvGetSize(img), img->depth, 1);
 	IplImage  *thres_img = cvCreateImage( cvGetSize(img), img->depth, 1);
	IplImage  *blobs_img = cvCreateImage( cvGetSize(img), img->depth, 3);

	/* Time */
	time_t current_time;
	current_time = time (NULL);
	long int stop_time = current_time + 10;

	/* initialize video writer */
	CvSize size;
        size.width = img->width;
        size.height = img->height;
       	double fps_Logitech = 11;
	CvVideoWriter *flirWriter = cvCreateVideoWriter("/home/sealab/Videos/flirOut.avi", CV_FOURCC('D','I','V','X'), fps_FLIR, size, 1); //needs time index
	
	/* main loop */
	CvScalar pixel;
	IplImage* moments_img = cvCreateImage( cvGetSize(blobs_img), blobs_img->depth, 1);//movement
	IplImage* temp_img = cvCreateImage( cvGetSize(blobs_img), blobs_img->depth, 3);//movement
	int i,j,k,pval,pvaltg,T,MG,areaInt;
  	int foundBlob=0,;
	bool Movement;
  	static int posX = 0;
  	static int posY = 0;
  	static int lastposX = 0;
  	static int lastposY = 0;
	CBlobResult blobs;
	CBlob bigblob;
	while(current_time < stop_time)
	{   
        	/* Obtain a frame from the device */
		img = cvQueryFrame( capture1 );

		pixel.val[0]=0.0;

		/* remove the watermark */
		for(j=372;j<455;j++){
		for(k=29;k<55;k++){
			cvSet2D(img, k, j, pixel);}}
		 
  		/* Convert image from Color to grayscale */
  		cvCvtColor(img,gray_img,CV_RGB2GRAY);

		/* Find Average */
	  	pvaltg=0;
  		for(j=44;j<470;j++)
		{
  		for(k=13;k<318;k++)
		{
			pixel = cvGet2D(gray_img, k, j);pval = pixel.val[0];
  			pvaltg=pvaltg+pval;
		}
		}
  		MG = pvaltg/129200;
  		T=1.8*MG;//set the threshold to 1.8*mean
  
		/* Apply the threshold */
  		cvThreshold(gray_img,thres_img,T,255,CV_THRESH_BINARY);
	
		/* Find Blobs that are White, Hence 'uchar backgroundColor = 0' (Black) */
		blobs = CBlobResult(thres_img, NULL, 0);

		/* Find biggest blob size */
  		blobs.GetNthBlob(CBlobGetArea(), 0, bigblob);
  		areaInt = bigblob.Area();
  	
		/* Remove blobs smaller than the biggest blob*/
		blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(),B_LESS, areaInt);
   	
		/* determin the center of blob and it's movement */

		cvCopy(blobs_img, temp_img);  
		cvSetImageCOI( temp_img, 3);
		cvCopy(temp_img, moments_img);
		CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
		cvMoments(moments_img, moments, 1);
		double moment10 = cvGetSpatialMoment(moments, 1, 0);
		double moment01 = cvGetSpatialMoment(moments, 0, 1);
		double area = cvGetCentralMoment(moments, 0, 0);
		posX = moment10/area;
		posY = moment01/area;


		if(blobs.GetNumBlobs()==0)
		{	
			posX = PicWidth/2; 
			posY = PicHeight/2;
			Movement = false;
		}
	
		else
		{
			//pub posX, posY, Height and width to service
			Movement = true;
		}
        
		/* Write images to file */
		cvWriteFrame(flirWriter, img);
 
	}
 	
	/* Clean up memory */
	cvReleaseCapture( &capture1 );
	cvReleaseVideoWriter(&flirWriter);

	return 0;
}

