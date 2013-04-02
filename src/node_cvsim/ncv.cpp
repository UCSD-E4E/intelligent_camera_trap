#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <iostream>

#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#include </opt/ros/groovy/include/opencv/cxcore.h>
#include </usr/local/include/cvblobs/BlobResult.h>
#include </usr/local/include/cvblob.h>
#include </usr/local/include/cvblobs/BlobOperators.h>

#include "ICT_Viper/CvService.h"
#include "parameters.h"
#include "karp.h"
#include "libcv_tracking.h"

int PicHeight = 320;
int PicWidth = 480;

class CVLocalizer 
{
		int x, y;
	public:
		CVLocalizer(int,int);
		void updateLocation(int updated_x, int updated_y)
		{
			x = updated_x;
			y = updated_y;
		};

		int getX(void)
		{
			return x;
		};

		int getY(void)
		{
			return y;
		}

		
		bool newCoords(ICT_Viper::CvService::Request &req, ICT_Viper::CvService::Response &res);


};

CVLocalizer::CVLocalizer(int x_init, int y_init)
{
	x = x_init;
	y = y_init;
}

bool CVLocalizer::newCoords(ICT_Viper::CvService::Request &req, ICT_Viper::CvService::Response &res)
{
	if (req.A == 0)
	{
		ROS_INFO("Recieved CV localization request\n");
		res.Coords = x;
		//res.Coords[1] = y;
	} 
	return true;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "cv_service");
	ros::NodeHandle n;

	CVLocalizer object_tracker(0,0);

	ros::ServiceServer service = n.advertiseService("cv_service", &CVLocalizer::newCoords, &object_tracker);

	//initialize cameras and capture feeds
	
/*	fd=open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);	
	fcntl(fd, F_SETFL,0);
	
	if (fd == -1){
		ROS_ERROR("Unable to open file descriptor for writing");
		return -1;
	}*/
	
	CvCapture *capture1 = 0;
	CvCapture *capture2 = 0;

	IplImage  *img = 0;
 	IplImage  *gray_img = 0;
 	IplImage  *thres_img = 0;
	IplImage  *blobs_img = 0;
	IplImage  *webcam_img = 0;

	CvVideoWriter *webcamWriter = 0;
	CvVideoWriter *flirWriter = 0;
	CvVideoWriter *blobsWriter = 0;

	int T,MG;
 
 	int frame_count= 0;
 	int i,j,k,pval,pvaltg;
// 	int index=0, indxtemp=0;
	int fps1=0, fps2=0;
	char movebuff[8];
	//double MovTurret[2];
	//CBlob biggestBlob;

	CvScalar pixel;

	/* set the norm to NTSC for FLIR - needs improvement - */
	system("v4lctl -c /dev/video0 setnorm NTSC");
	system("v4lctl -c /dev/video1 setnorm NTSC");
	system("v4lctl -c /dev/video2 setnorm NTSC");
	
	/* Initialize the webcam */

	//sweep through possible device indicesi
	int index = 0;
	
	int indxtemp = 0;
	for (index=0;index<10;index++)
	{
		capture1 = cvCaptureFromCAM(index);
		indxtemp=index+1;
		if (capture1) break;
	}
	
	for (index=indxtemp;index<10;index++)
	{
		capture2 = cvCaptureFromCAM(index);
		if (capture2) break;
	}
	//Figure out which device is the webcam, which is the thermal cam
	cvQueryFrame(capture1);
	fps1 = cvGetCaptureProperty(capture1, CV_CAP_PROP_FRAME_WIDTH);
   
	cvQueryFrame(capture2);
	fps2 = cvGetCaptureProperty(capture2, CV_CAP_PROP_FRAME_HEIGHT);

	if(fps1 < fps2)
	{
		cvSetCaptureProperty(capture1, CV_CAP_PROP_FRAME_WIDTH, 480);
		cvSetCaptureProperty(capture1, CV_CAP_PROP_FRAME_HEIGHT, 320);
		cvSetCaptureProperty(capture1, CV_CAP_PROP_FPS, 24);	

 		cvSetCaptureProperty(capture2, CV_CAP_PROP_FRAME_WIDTH, 960);
		cvSetCaptureProperty(capture2, CV_CAP_PROP_FRAME_HEIGHT, 720);
		cvSetCaptureProperty(capture2, CV_CAP_PROP_FPS, 30);	
	}
	else
	{
		cvSetCaptureProperty(capture2, CV_CAP_PROP_FRAME_WIDTH, 480);
		cvSetCaptureProperty(capture2, CV_CAP_PROP_FRAME_HEIGHT, 320);
		cvSetCaptureProperty(capture2, CV_CAP_PROP_FPS, 24);	

 		cvSetCaptureProperty(capture1, CV_CAP_PROP_FRAME_WIDTH, 960);
		cvSetCaptureProperty(capture1, CV_CAP_PROP_FRAME_HEIGHT, 720);
		cvSetCaptureProperty(capture1, CV_CAP_PROP_FPS, 30);	
	}
       
	/* Always check if the program can find a device */
        if (!capture1)
	{
        	ROS_ERROR("Could not open video1");
		return -1;
	}

        if (!capture2)
	{
        	ROS_ERROR("Could not open video2");
		return -1;
	}
	
	
	//CBlobResult blobs;
 	//CBlob *currentBlob;
  	
	/* Create required images once */
 	if( frame_count == 0 )
 	{
        
		if(fps1 < fps2)
		{
			img = cvQueryFrame( capture1 );
			webcam_img = cvQueryFrame( capture2 );
		}
		else
		{
			img = cvQueryFrame( capture2 );
			webcam_img = cvQueryFrame( capture1 );
		}

  		// Always check if the device returns a frame 
  		if( !img ){
   			ROS_ERROR("Error retrieving FLIR frame\n");
			return -1;
		}
 
		webcam_img = cvQueryFrame(capture2);
		
		if (!webcam_img)
		{
        		ROS_ERROR("Error retrieving webcam frame\n");
			return -1;
       		}

  		gray_img  = cvCreateImage( cvGetSize(img), img->depth, 1);
  		thres_img = cvCreateImage( cvGetSize(img), img->depth, 1);
  		blobs_img = cvCreateImage( cvGetSize(img), img->depth, 3);
	}
	frame_count = frame_count + 1;

		//initialize video writer
	CvSize size;
        size.width = webcam_img->width;
        size.height = webcam_img->height;

        double fps_Logitech = 11;

        webcamWriter = cvCreateVideoWriter("/home/sealab/Videos/cameraOut.avi", CV_FOURCC('D','I','V','X'), fps_Logitech, size, 1);

	//initialize blobs and flir video writers 
	size.width = img->width;
	size.height = img->height;

        double fps_FLIR = 9;
		
	flirWriter = cvCreateVideoWriter("/home/sealab/Videos/flirOut.avi", CV_FOURCC('D','I','V','X'), fps_FLIR, size, 1);
	blobsWriter = cvCreateVideoWriter("/home/sealab/Videos/blobsOut.avi", CV_FOURCC('D','I','V','X'), fps_FLIR, size, 1);
	
  	int foundBlob=0;
  	static int posX = 0;
  	static int posY = 0;
  	//static int lastposX = 0;
  	//static int lastposY = 0;
	//int pl=0;
	IplImage* imgScribble = NULL;

	//long int frameCounter = 0;
	//char filename[50];	

	//timing here	

	while (ros::ok())
	{
		if(fps1 < fps2)
		{
			img = cvQueryFrame( capture1 );
         webcam_img = cvQueryFrame( capture2 );
		}
		else
		{
			img = cvQueryFrame( capture2 );
			webcam_img = cvQueryFrame( capture1 );
		}

		img  = removeWatermark(img);
		
		if( !img )
		{
			ROS_ERROR("Could not get a frame from the FLIR\n");
			return -1;
		}

		if (!webcam_img)
		{
			ROS_ERROR("Could not get a frame from the Webcam\n");
			return -1;
		}

		if(imgScribble == NULL)
		{
			imgScribble = cvCreateImage(cvGetSize(img), 8, 3);          
		}


		/* Convert image from Color to grayscale */
/*		cvCvtColor(img,gray_img,CV_RGB2GRAY);

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
		 
		// Apply the threshold 
		cvThreshold(gray_img,thres_img,T,255,CV_THRESH_BINARY);

		// Find Blobs that are White, Hence 'uchar backgroundColor = 0' (Black) 
		blobs = CBlobResult(thres_img, NULL, 0);

		// Find biggest blob size 
		CBlob bigblob;
      blobs.GetNthBlob(CBlobGetArea(), 0, bigblob);
		int areaInt;
		areaInt = bigblob.Area();

		// Remove blobs smaller than the biggest blob
		blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(),B_LESS, areaInt);

		if (foundBlob == 0 && blobs.GetNumBlobs()>0)
		{
			ROS_INFO("*******TARGET AQUIRED********\n");
			foundBlob=1;
			write(fd, &start_cmd, 4);
		}
                
		if(foundBlob)
			ROS_INFO("*******TARGET AQUIRED********\n");
		
	// determine the center of blob and it's movement 
                
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
		//int lastX = posX;
		//int lastY = posY;
		//lastposX = posX;
		//lastposY = posY;
		posX = moment10/area;
		posY = moment01/area;


                
		if(blobs.GetNumBlobs()==0)
		{                  
			posX = PicWidth/2;
			posY=PicHeight/2;
			
			         
		}

                
		
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

		char* moveptr = degrees_to_serial(movebuff, turn_turret, vx/2.5, vy/2.5);
            
		for (i=0;i<8;i++)
		{
			movebuff[i] = *(moveptr + i);
		}

		write(fd, movebuff, 8);
		cvAdd(img, imgScribble, img);
		*/                
		/* Publishing videos */
		cv::Mat flir(img);
		cv::Mat webcam(webcam_img);
		cv::imshow("Webcam", webcam);
		cv::imshow("FLIR", flir);
		cv::waitKey(1);

                
					
		/* Write images to file */
      		cvWriteFrame(webcamWriter, webcam_img);
		cvWriteFrame(flirWriter, img);
		//cvWriteFrame(blobsWriter, blobs_img);
		foundBlob=0;

 		/* Clear image for next iteration */
		cvZero(img);
		cvZero(gray_img);
		cvZero(thres_img);
		//cvZero(blobs_img);
		cvZero(webcam_img);
		cvZero(imgScribble);
		}
 	/* Clean up memory */
	cvReleaseCapture( &capture1 );
	cvReleaseCapture( &capture2 );
	cvReleaseVideoWriter(&webcamWriter);
	cvReleaseVideoWriter(&blobsWriter);
	cvReleaseVideoWriter(&flirWriter);
	return 0;
}


