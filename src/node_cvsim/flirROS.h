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
#include </opt/ros/fuerte/include/opencv/cxcore.h>
#include </usr/local/include/cvblobs/BlobResult.h>
#include </opt/ros/fuerte/include/opencv/cvblob.h>
#include </usr/local/include/cvblobs/BlobOperators.h>

#include "parameters.h"
#include "karp.h"

int PicHeight = 240;
int PicWidth = 320;

void GetDegreesFromCenter(int x, int y,double out[2] ){

	out[0] = ((double)x-PicWidth/2)/PicWidth/2; //+1 cuz pixel counting starts at 0 but width starts at 1
	out[1] = ((double)y-PicHeight/2)/PicHeight/2;

}

int trackObject()
{
    CvCapture *capture = 0;
    IplImage  *img = 0;
 	IplImage  *gray_img = 0;
 	IplImage  *thres_img = 0;
    IplImage  *blobs_img = 0;
 	int key = 0;
	char movebuff[8];
	double MovTurret[2];
	CBlob biggestBlob;

    /* Initialize the webcam */
    //capture = cvCreateCameraCapture(0);
	capture = cvCaptureFromCAM(0);
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 320);
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
  	
    /* Always check if the program can find a device */
    if ( !capture )
        return -1; 
 
 	CBlobResult blobs;
 	CBlob *currentBlob;
 
 	int frame_count= 0;
 	int i;
  
    /* create a window for the input and output video */
 	cvNamedWindow( "FLIR",CV_WINDOW_AUTOSIZE);//from IR camera
	cvNamedWindow( "Blobs",CV_WINDOW_AUTOSIZE);//blobs output
  	//cvNamedWindow("test", CV_WINDOW_AUTOSIZE);
 	/* Create required images once */
 	if( frame_count == 0 )
  {
        /* Obtain a frame from the device */
        img = cvQueryFrame( capture );
 		//printf("Img height == %d\n", img->height);
  		/* Always check if the device returns a frame */
  		if( !img ){
   			ROS_ERROR("Could not open capture device\n");
			return -1;
		}
 
  		gray_img  = cvCreateImage( cvGetSize(img), img->depth, 1);
  		thres_img = cvCreateImage( cvGetSize(img), img->depth, 1);
  		blobs_img = cvCreateImage( cvGetSize(img), img->depth, 3);
	
 	}
  	int foundBlob=0;
  	static int posX = 0;
  	static int posY = 0;
  	static int lastposX = 0;
  	static int lastposY = 0;
  int pl=0;
	IplImage* imgScribble = NULL;

  long int frameCounter = 0;
  char filename[50];

  time_t current_time;
  current_time = time (NULL);
  long int stop_time = current_time + 10;

while(current_time < stop_time){
   
        /* Obtain a frame from the device */
        img = cvQueryFrame( capture );
 
  	   /* Always check if the device returns a frame */
  	   if( !img )
   		   return -1;
	   if(imgScribble == NULL)
     {
		    imgScribble = cvCreateImage(cvGetSize(img), 8, 3);
     }
   
  	 frame_count = frame_count + 1;
   
  	/* Convert image from Color to grayscale and then to binary (thresholded at 200) */
  	cvCvtColor(img,gray_img,CV_RGB2GRAY);
  	cvThreshold(gray_img,thres_img,120,128,CV_THRESH_BINARY);
   	if (foundBlob == 0)
	  {
		  if (blobs.GetNumBlobs()>0)
		  {	
			 ROS_INFO("****************************\n*******TARGET AQUIRED********\n****************************");
			 foundBlob=1;
			 write(fd, &start_cmd, 4);
       //ROS_INFO("*****************************\n*******TARGET AQUIRED********\n*****************************\n");
		  }
	  }
	  	
	/* Find Blobs that are White, Hence 'uchar backgroundColor = 0' (Black) */
  /* Find Blobs that are White, Hence 'uchar backgroundColor = 0' (Black) */
  blobs = CBlobResult(thres_img, NULL, 0);
 
  /* Remove blobs if it does not cover minimum area specified below */
  blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(),B_LESS,200);
 
  /* Color the blobs one after the other */
  for (i = 0; i < blobs.GetNumBlobs(); i++ )
  {
   currentBlob = blobs.GetBlob(i);
   //currentBlob->FillBlob( blobs_img, CV_RGB(255,204,0));//yellow
   currentBlob->FillBlob( blobs_img, CV_RGB(255,255,255));//white
  }
/* determin the center of blob and it's movement */
  IplImage* moments_img = cvCreateImage( cvGetSize(blobs_img), blobs_img->depth, 1);
  IplImage* temp_img = cvCreateImage( cvGetSize(blobs_img), blobs_img->depth, 3);
  cvCopy(blobs_img, temp_img);  
  cvSetImageCOI( temp_img, 3);
  cvCopy(temp_img, moments_img);
  CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
  cvMoments(moments_img, moments, 1);
  double moment10 = cvGetSpatialMoment(moments, 1, 0);
  double moment01 = cvGetSpatialMoment(moments, 0, 1);
  double area = cvGetCentralMoment(moments, 0, 0);
  int lastX = posX;
  int lastY = posY;
  lastposX = posX;
  lastposY = posY;
  posX = moment10/area;
  posY = moment01/area;


if(blobs.GetNumBlobs()==0)
{	
	posX = PicWidth/2; 
	posY=PicHeight/2;
	MovTurret[0]=0; 
	MovTurret[1]=0;
}
	
	/* make the text output for movement */
	if( posX>0 && posY>0)
	{
	    // Draw a yellow line from the previous point to the current point
	    cvLine(imgScribble, cvPoint(posX, posY), cvPoint(posX,posY), cvScalar(0,255,255), 5);
	}  

	double vx, vy;
	vx = (100*(posX-PicWidth/2)/(PicWidth/2));
	vy = (100*(posY-PicHeight/2)/(PicHeight/2));
	
	if ((vx*vx)<=(X_OFFSET_TOLERANCE*X_OFFSET_TOLERANCE))
		vx = 0;
	if ((vy*vy)<=(Y_OFFSET_TOLERANCE*Y_OFFSET_TOLERANCE))
		vy = 0;
	
	if (foundBlob==1)
	{
		write(fd, &start_cmd, 4);	
	}

	char* moveptr = degrees_to_serial(movebuff, turn_turret, vx/2, vy/2);
	    for (i=0;i<8;i++)
	    {
		    movebuff[i] = *(moveptr + i);
	    }
	    
	    	write(fd, movebuff, 8);

	    // printf("writing to serial:\n");
	    // for (i=0;i<8;i++)
	    // {
		   //  printf("%c", movebuff[i]);
	    // }
	    // printf("\n");
  	// Add the scribbling image and the frame...
        cvAdd(img, imgScribble, img);
        
    current_time = time (NULL);
    ROS_INFO("time remaining = %ld\n", (long int)(stop_time-curernt_time);
    
    if ((stop_time-current_time <= 5)&&(blobs.GetNumBlobs()>0))
    {
      stop_time += 10;
    }

	 /* Show images and move the windows */
	cvShowImage( "Blobs",blobs_img);
	cvMoveWindow( "Blobs",300,250);
	cvShowImage( "FLIR",img);
	cvMoveWindow( "FLIR",625,250);
	foundBlob=0;
	sprintf(filename, "./blobs%06ld.jpeg", frameCounter); 
	cvSaveImage(filename, blobs_img);
	sprintf(filename, "./flir%06ld.jpeg", frameCounter++);
	cvSaveImage(filename, img);
 
  /* Clear image for next iteration */
  cvZero(blobs_img);
  cvZero(imgScribble);
   
  
  }
 
 /* Clean up memory */
    cvDestroyWindow( "FLIR" );
    cvDestroyWindow( "Blobs" );
    cvReleaseCapture( &capture );
    return 0;
}