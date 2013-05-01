#include <stdio.h>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
 
int main(int argc, char * const argv[])
{
	/* Initialize the camera */
	CvCapture *CamCapture = 0;
	CamCapture = cvCreateFileCapture("http://192.168.2.135:81/videostream.asf?user=viki&pwd=viki&resolution=640*480");
	if (!CamCapture)
	{printf("IP Cam not ready\n");
	return -1;}
	/* initialize video writer */
	CvVideoWriter *IPCamWriter = 0;
	CvSize size;
	size.width = 640;
	size.height = 480;
	IPCamWriter = cvCreateVideoWriter("/home/viki/Videos/IPCamOut.avi", CV_FOURCC('D','I','V','X'), 4, size, 1); //needs time index in naming

	/* time */
	time_t current_time;
	current_time = time (NULL);
	long int stop_time = current_time + 10;
	
	/* main loop */
	while( current_time < stop_time )
		{
		/* write image to file */
		cvWriteFrame(IPCamWriter, cvQueryFrame( CamCapture ));
		}
 
	/* Clean up memory */
	cvReleaseCapture ( &CamCapture );
	cvReleaseVideoWriter(&IPCamWriter);
}
