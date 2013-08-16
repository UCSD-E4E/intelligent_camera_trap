/*
 * Run Mixture Of Gaussians (MOG) 2 from openCV
 * on a specified video input sequence.  
 * Find and display the centroids of those foreground frames.
 * (Or...the commented section of the code runs connected
 * components on the foreground image.)
 * (GRAYSCALES THE GIVEN INPUT SEQUENCE).
 */


#include<opencv2/opencv.hpp>
#include<iostream>
#include <vector>
using namespace cv;
using namespace std;


/*
 * Get the centroid of our binary images.
 */
Point getCentroid(cv::Mat img)
{
    Point coord;
    Moments mm = moments(img,true);
    double moment10 = mm.m10;
    double moment01 = mm.m01;
    double moment00 = mm.m00;
    coord.x = int(moment10 / moment00);
    coord.y = int(moment01 / moment00);
    return coord;
}



int main(int argc, char *argv[])
{
	Mat frame;
    Mat gray;
	Mat back;
	Mat fore;
    Mat blob_image;
    int frame_count = 0;
	VideoCapture cap("/media/MEMORY/file_3_low_resolution.mov");
	if(!cap.isOpened())
	{
		cout<<"Could not open the specified video capture.";
		return -1;
	}
	else
	{
		cout<<"Opened the video capture.";
	}
	BackgroundSubtractorMOG2 bg;
    vector<vector<Point> > contours;
    Point p;
    Point last_center;
    Scalar color = Scalar(255,255,255);
    double contour_size;
    int largest_area = 0;
    int i, largest_contour_index = 0;
    int x, y = 0;

	namedWindow("Frame",CV_WINDOW_NORMAL);
	namedWindow("Foreground",CV_WINDOW_NORMAL);
	namedWindow("Blob",CV_WINDOW_NORMAL);
 

	
	for(;;)
	{
        //cout<<frame_count<<endl;
		cap >> frame;
        cvtColor(frame, gray, CV_BGR2GRAY);
		bg.operator()(gray,fore);
        imshow("Frame",frame);        
        
        //find the centroid of the foreground
        last_center = getCentroid(fore);
        x += last_center.x;
        y += last_center.y;
        if (frame_count % 10 == 0)  //average 10 centroids, draw a cirlce where it's at
        {
            x = int(x/10.);
            y = int(y/10.);
            last_center.x = x;
            last_center.y = y;
            circle(fore, last_center, 15, color, -1, 8, 0);
            x = 0;
            y = 0;
        }
        if (frame_count >= 10)
        {
            circle(fore, last_center, 15, color, -1, 8, 0);
            imshow("Foreground",fore); 
        }
        
        //connected components: (contours in openV)
        /*
        blob_image = Mat::zeros(frame.rows, frame.cols, CV_8UC3);       
        findContours(fore, contours, CcontourV_RETR_LIST, CV_CHAIN_APPROX_NONE);
        largest_area = 0;         
        for(i = 0; i < contours.size(); ++i)
        {     
            contour_size = contourArea(contours[i],false);
            if( contour_size > largest_area)
            {
                largest_area = contour_size;
                largest_contour_index = i;
            }          

        }
        //cout<<"Num Contours: "<<contours.size()<<endl;
        
        if( contours.size() >= 1)
        {  
            //cout<<"largest contour index = "<<largest_contour_index<<endl;
            drawContours(blob_image, contours, largest_contour_index, color, -1, 8);
        }        
        imshow("Blob",blob_image);   
        */
        if( waitKey(30) >= 0) break;
        ++frame_count;
	}
	return 0;
}
