#include "ros/ros.h"
//#include "service protocol definition here"

//service callback return function

int main(int argc, char **argv)
{
	ros::init(argc, argv, "cv_service");
	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("cv_service", localize);

	//initialize cameras and capture feeds
	
	fd=open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);	
	fcntl(fd, F_SETFL,0);
	
	if (fd == -1){
		ROS_ERROR("Unable to open file descriptor for writing");
		return -1;
	}
	
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

	int T,MG,TG;
 
 	int frame_count= 0;
 	int i,j,k,pval,pvaltg;
 	int key = 0, index=0, indxtemp=0;
	int fps1=0, fps2=0;
	char movebuff[8];
	double MovTurret[2];
	CBlob biggestBlob;

	CvScalar pixel;

	/* set the norm to NTSC for FLIR - needs improvement - */
	system("v4lctl -c /dev/video0 setnorm NTSC");
	system("v4lctl -c /dev/video1 setnorm NTSC");
	system("v4lctl -c /dev/video2 setnorm NTSC");
	
	/* Initialize the webcam */

	//sweep through possible device indices
	for (index=0;index<10;index++)
	{
		capture1 = cvCaptureFromCAM(index);indxtemp=index+1;
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
	
	
	CBlobResult blobs;
 	CBlob *currentBlob;
  	
	/* Create required images once */
 	if( frame_count == 0 )
 	{
        /* Obtain a frame from the device */
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

  		/* Always check if the device returns a frame */
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
  	static int lastposX = 0;
  	static int lastposY = 0;
	int pl=0;
	IplImage* imgScribble = NULL;

	long int frameCounter = 0;
	char filename[50];	

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

		/* Convert image from Color to grayscale */
                cvCvtColor(img,gray_img,CV_RGB2GRAY);


		
	}

