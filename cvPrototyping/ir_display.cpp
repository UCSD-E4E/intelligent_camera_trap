#include <opencv2/opencv.hpp>
#include <iostream>


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


int main(int argc, char *argv[])
{
    Mat frame1;
    Mat frame;
    Mat fore;
    Mat biggest_contour;

    const int FRAME_W = 16;
    const int FRAME_H = 4;

    namedWindow("raw", CV_WINDOW_AUTOSIZE);
    namedWindow("frame", CV_WINDOW_AUTOSIZE);

    BackgroundSubtractorMOG2 bg = BackgroundSubtractorMOG2(200, 16, false);
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

    // Initialize Melexis Sensor (I2c and whatnot)
    // GOES HERE


    // tracking loop
    while( 1 )
    {

        // Get a frame from the melexis sensor

        uint8_t ir_frame[4][16];
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 16; j++)
            {
                ir_frame[i][j] = 10*(i+j);
            }
        }

        frame = Mat(4, 16, CV_8UC1, &ir_frame);

        // color
        bg.operator()(frame, fore, 0.002);

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

        biggest_contour = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
        drawContours( biggest_contour, contours, larg_contour_index, color, -1, 8);
        last_center = get_centroid(biggest_contour);


        Mat large_frame;
        resize(frame, large_frame, Size(16*FRAME_W, 16*FRAME_H), 0, 0, INTER_NEAREST);

        imshow("frame", large_frame);


        state = 1;

    // cout << " (teta,state) = " << "(" << cpolar.y << "," << state << ")" << endl;

        ++frame_count;
        waitKey(1);
        //if( waitKey(1) >= 0) break;
    }
    return 0;
}
