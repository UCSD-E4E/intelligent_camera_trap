#include <opencv2/opencv.hpp>
#include <iostream>
#include "bbb_i2c.h"

using namespace cv;
using namespace std;

#define PI 3.14159

void print_frame(uint8_t (*frame_in)[16])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            cout << frame_in[i][j] << ", ";
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

    BackgroundSubtractorMOG2 bg = BackgroundSubtractorMOG2(1, 16, false);
    vector<vector<Point> > precontours;
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
    INITIALIZE();


    // tracking loop
    while( 1 )
    {

        // Get a frame from the melexis sensor

        uint8_t ir_frame[4][16];

        frame(ir_frame);
        cv_frame = Mat(4, 16, CV_8UC1, &ir_frame);

        bg.operator()(cv_frame, fore, 0.1);

        // find all contours, get all the points in each contour
        findContours(fore, precontours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

        contours = precontours;

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

        biggest_contour = Mat::zeros(cv_frame.rows, cv_frame.cols, CV_8UC1);
        drawContours( biggest_contour, contours, larg_contour_index, color, -1, 8);
        last_center = get_centroid(biggest_contour);

        Mat large_frame;
        Mat large_thresh;
        resize(cv_frame, large_frame, Size(16*FRAME_W, 16*FRAME_H), 0, 0, INTER_NEAREST);
        resize(fore, large_thresh, Size(16*FRAME_W, 16*FRAME_H), 0, 0, INTER_NEAREST);

  //      imshow("raw", large_frame);
  //      imshow("frame", large_thresh);


        state = 1;

        print_frame(ir_frame);
//        cout << "Center position: ";
        ++frame_count;
    }
    return 0;
}



