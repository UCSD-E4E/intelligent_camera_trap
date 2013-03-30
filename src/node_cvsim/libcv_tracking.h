#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <iostream>
#include </opt/ros/groovy/include/opencv/cv.h>
#include </opt/ros/groovy/include/opencv/highgui.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include </opt/ros/groovy/include/opencv/cxcore.h>
#include </usr/local/include/cvblobs/BlobResult.h>
#include </opt/ros/groovy/include/opencv/cvblob.h>
#include </usr/local/include/cvblobs/BlobOperators.h>
#include "parameters.h"


IplImage *removeWatermark(IplImage *input_img){
	CvScalar pixel;
	int j;
	int k;
	
	pixel.val[0] = 0.0;

	/*remove FLIR logo watermark*/
 	for(j=372;j<455;j++)
	{
                for(k=29;k<55;k++)
		{
                        cvSet2D(input_img, k, j, pixel);
		}
	}

	/* remove the line in corner */
	for(j=0;j<30;j++)
	{
                for(k=0;k<320;k++)
		{
                        cvSet2D(input_img, k, j, pixel);
		}
	}
	return input_img;
}
