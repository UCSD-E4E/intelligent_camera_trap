#include<opencv2/opencv.hpp>
#include<iostream>
#include <vector>
#include <time.h>
#include <string>
#include <cstdlib>
#include <math.h>
#include "/home/sealab/Desktop/motor_controller.h"

using namespace cv;
using namespace std;

#define PI 3.14159


/*
 * Return the centroid of a segmented binary image.
 */
Point get_centroid(Mat img)
{
    Point coord;
    Moments mm = moments(img,false);
    double moment10 = mm.m10;
    double moment01 = mm.m01;
    double moment00 = mm.m00;
    coord.x = int(moment10 / moment00);
    coord.y = int(moment01 / moment00);
    return coord;
}



/*
 * Control the turret based on the passed
 * in x and y coordinates.
 */
void turret_control(Point coordinates, MotorController *mctrl, const int frame_width, const int frame_height)
{
    int x;
    int y; 
    double x_deg;   
    
    // adjust x and y coordinates
    x = coordinates.x - (frame_width/2);
    y = ((-1 * coordinates.y) + (frame_height/2));    

    if( y > frame_height/8 )
    {
        x_deg = (180 + (int)(atan2(y,x)*180/(PI))) % 180;
        mctrl->new_pan = x_deg;
        mctrl->new_tilt = mctrl->tilt_pos;
        mctrl->updatePosition();
    }

}






int main(int argc, char *argv[])
{
    Mat frame;
    Mat gray;
    Mat fore;
    Mat biggest_contour;

    const int FRAME_W = 160;
    const int FRAME_H = 120;
 
    BackgroundSubtractorMOG2 bg;
    vector<vector<Point> > contours;
    Point last_center;
    Scalar color = Scalar(255,255,255);    
    unsigned long frame_count = 0;
    unsigned long largest_size = 0;  //biggest contour
    int contour_size = 0;
    int i, largest_contour_index = 0;
    

    
    // open the 360 cam, and set it's resolution
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
        cout << endl << "Failed to connect to the 360 camera." << endl << endl;
    }
    else
    {
        cout << endl << "Connected to 360 camera." << endl << endl;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_W);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_H);

    // open the motor controller
    MotorController mctrl("/dev/ttyUSB0", 19200, 0.0, 0.0);
    mctrl.updatePanTilt();
    cout << "Motor controller created" << endl;
   
    // tracking loop
    for(;;)
    {
        cap >> frame;
        cout << "Frame number: " <<frame_count << endl;
        
        cvtColor(frame, gray, CV_BGR2GRAY); //grayscale the frame
        bg.operator()(gray,fore);           //get the binary foreground image
        
        // find all contours, get all the points in each contour
        findContours(fore, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE); 

        // find largest contour
        largest_size = 0;
        for( i = 0; i < contours.size(); ++i )
        {
            contour_size = contours[i].size();
            if( contour_size > largest_size )
            {
                largest_size = contour_size;
                largest_contour_index = i;
            }
        }


        // needs to be at least one contour for us to choose a new tracking point
        if( contours.size() > 1 )
        {
            // isolate largest contour in its own Mat
            biggest_contour = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
            drawContours( biggest_contour, contours, largest_contour_index, color, 1, 8);

            last_center = get_centroid(biggest_contour);
            cout << "X = " << last_center.x;
            cout << "  fY = " << last_center.y;
            
            // move the turret:
            turret_control(last_center, &mctrl, FRAME_W, FRAME_H);
        }


        cout <<endl<<endl;
        ++frame_count;  

        if( waitKey(1) >= 0) break;
    }
return 0;
}
