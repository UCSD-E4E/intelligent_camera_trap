#include <opencv2/opencv.hpp>
#include <iostream>
#include "bbb_2_sensors.h"

using namespace cv;
using namespace std;

#define PI 3.14159

void print_frame(uint8_t (*frame_in)[16])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            printf("%d, ", frame_in[i][j]);
        }
        cout << "\n";
    }
    cout << "\n \n";
}

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

void mat_to_uint_array(Mat src, uint8_t (*dst)[16], int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            dst[i][j] = src.data[i*columns + j];
        }
    }
}

int get_degrees(int x_pos)
{
    int degrees = 60*x_pos/15 + 60;
    return degrees;
}

int main(int argc, char *argv[])
{
    Mat frame1;
    Mat cv_frame;
    Mat fore;
    Mat biggest_contour;

    const int FRAME_W = 16;
    const int FRAME_H = 4;

//    namedWindow("raw", CV_WINDOW_AUTOSIZE);
//    namedWindow("frame", CV_WINDOW_AUTOSIZE);

    BackgroundSubtractorMOG2 bg = BackgroundSubtractorMOG2(1, 0, false);
    vector<vector<Point> > ir_contours;
    vector<vector<Point> > contours;
    Point last_center;
    Scalar color = Scalar(255,255,255);
    unsigned long frame_count = 0;
    unsigned long largest_size = 0;  //biggest contour
    int contour_size = 0;
    int i = 0, j = 0;
    int larg_contour_index;

    int avg, wit = 0, state = 0, avrg[75] , k = 0;
    for ( j=0;j<75;++j ) { avrg[j] = 0; }

    // Initialize Melexis Sensor (I2C and whatnot)
    INITIALIZE(2);


    // tracking loop
    while( 1 )
    {

        // Get a frame from the melexis sensor

        uint8_t ir_frame[4][16];
        uint8_t ir_thresh[4][16];
        frame(ir_frame,2);
        cv_frame = Mat(4, 16, CV_8UC1, &ir_frame);

        //print_frame(ir_thresh);

	threshold(cv_frame, fore, 220, 255, THRESH_BINARY);
	
	
	mat_to_uint_array(fore, ir_thresh, 4, 16);
        
//        bg.operator()(ir_thresh, fore, 0.1);


        // find all contours, get all the points in each contour
        findContours(fore, ir_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

        contours = ir_contours;

        // find largest contour
        largest_size = 0;
        larg_contour_index = 0;
        for( i = 0; i < contours.size(); ++i )
        {
            contour_size = contours[i].size();
            if( contour_size > largest_size )
            {
                largest_size = contour_size;
                larg_contour_index = i;
            }
        }
        

/*        biggest_contour = Mat::zeros(4, 16, CV_8UC1);
  
        drawContours( biggest_contour, contours, larg_contour_index, color, -1, 8);
        last_center = get_centroid(biggest_contour);
*/


        state = 1;

	

        // Add back in code which says where to turn to and what state
        // How to do this is in cvPrototyping/remote_system.cpp

        biggest_contour = Mat::zeros(4, 16, CV_8UC1);
        
	if( contours.size() > 0)
        {
            // isolate largest contour in its own Mat
            drawContours( biggest_contour, contours, larg_contour_index, color, -1, 8);
            last_center = get_centroid(biggest_contour);

        }
        else
        {
        //    cout << endl << " no contour";
        }
        mat_to_uint_array(biggest_contour, ir_thresh, 4, 16);
    //    print_frame(ir_thresh);
/*
        //decision making for target state

        if ( k > 74 )
            k = 0; //movement in the last 5 seconds

        avrg[k] = cpolar.y;
        ++k;
        avg = 0;
        for ( j=0;j<75;++j)
            avg += avrg[j];

        if ( cpolar.y < 5 || cpolar.y > 175 ) //if target is out of view (teta < 5 or teta > 175)
        {
            if( wit > 100 )
                state = 0; //wait about a min before going to 0
            else
              ++wit;
        }
        else //if target is in view (5 < teta < 175)
        {
            if ( cpolar.y == avg/75 ) //if target is not moving for 5 sec
            {
                if ( wit > 100 )
                    state = 0; //wait about a min before going to 0
            else
              ++wit;
            }
            else
            {
              state = 1;
              wit = 0;
            } //if target is moving in the view area record and set the wait to 0
        }
*/
        int theta = get_degrees(last_center.x);
        cout << " (theta,state) = " << "(" << theta << "," << state << ")" << endl;



        ++frame_count;
    }
    return 0;
}



