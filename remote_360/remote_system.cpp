#include <opencv2/opencv.hpp>
#include "cvx_defs.h"
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

Point get_polar(int argx, int argy, int framew)
{
    Point polar;
    polar.x = sqrt(pow(argx,2) + pow(argy,2));
    polar.y = atan2(argy, (argx-framew/2)) * 180 / PI;
    return polar;
}

int main(int argc, char *argv[])
{
    Mat frame1;
    Mat frame;
    Mat gray;
    Mat fore;
    Mat biggest_contour;

    Rect roi;

    CvKalman* kalman = cvCreateKalman( 2, 1, 0 );//state, measurement, control
    CvRandState rng;
    cvRandInit(&rng,0,1,-1,CV_RAND_UNI);
    cvRandSetRange( &rng, 0, 0.1, 0 );
    rng.disttype = CV_RAND_NORMAL;
    CvMat* kmes = cvCreateMat(1, 1, CV_32FC1);//measurement, just angle
    cvZero(kmes);
    const float F[] = {1,1,0,1};//transition matrix describes model parameters at k and k+1
    memcpy( kalman->transition_matrix->data.fl, F, sizeof(F));//dest, src, #of bytes
    cvSetIdentity( kalman->measurement_matrix,    cvRealScalar(1) );//matPtr, value
    cvSetIdentity( kalman->process_noise_cov,     cvRealScalar(1e-5) );
    cvSetIdentity( kalman->measurement_noise_cov, cvRealScalar(1e-1) );
    cvSetIdentity( kalman->error_cov_post,        cvRealScalar(1));
    cvRand(&rng, kalman->state_post);//choose random initial state
    Scalar red = CV_RGB(255,0,0);
    Scalar blue = CV_RGB(0,0,255);
    Mat dummy;
    int value = 0;

    //namedWindow("kalman", CV_WINDOW_AUTOSIZE);
    //namedWindow("frame", CV_WINDOW_AUTOSIZE);
    const int FRAME_W = 640;
    const int FRAME_H = 480;
 
    BackgroundSubtractorMOG2 bg;
    vector<vector<Point> > precontours;
    vector<vector<Point> > contours;
    Point last_center;
    Point cpolar;
    Scalar color = Scalar(255,255,255);
    unsigned long frame_count = 0;
    unsigned long largest_size = 0;  //biggest contour
    int contour_size = 0;
    int i = 0, j = 0;
    int larg_contour_index;
    int angl, rad;

    int avg, wit = 0, state = 0, avrg[75] , k = 0;
    for ( j=0;j<75;++j ) { avrg[j] = 0; }

    
    // open the 360 cam, and set it's resolution
    VideoCapture cap("/home/ubuntu/remote_360/video.avi");
    if(!cap.isOpened())
    {
        cout << endl << "Failed to open /home/ubuntu/remote_360/video.avi." << endl << endl;
    }
    else
    {
        cout << endl << "Connected to 360 camera." << endl << endl;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_W);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_H);
   
    // tracking loop
    while( 1 )
    {
        cap >> frame1;
        //cout << "Frame number: " <<frame_count << endl;
	roi = Rect(160, 250, 300, 150);
	frame = frame1(roi);

        cvtColor(frame, gray, CV_BGR2GRAY); //grayscale the frame
        bg.operator()(gray,fore);           //get the binary foreground image

        // find all contours, get all the points in each contour
        findContours(fore, precontours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE); 

        // filter small contours
	contours = precontours;
	j = 0;
	for( i = 0; i < precontours.size(); ++i )
        {
            contour_size = precontours[i].size();
            if( contour_size < 5 )
            {
                contours.erase(contours.begin()+i-j);
		++j;
            }
        }
	

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

        // needs to be at least one contour for us to choose a new tracking point
        if( contours.size() > 1 )
        {
            // isolate largest contour in its own Mat
            biggest_contour = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
            drawContours( biggest_contour, contours, larg_contour_index, color, 1, 8);
            last_center = get_centroid(biggest_contour);
	    cpolar = get_polar (last_center.x, last_center.y, frame.cols);
	    
	    //filter based on Kalman's output


	    //kalman
	    const CvMat *kprd= cvKalmanPredict(kalman,0);//predict pos. kalman, control
	    cvSetReal1D(kmes, 0, cpolar.y);
	    dummy = cvarrToMat(kprd);
	    value = dummy.at<float>(0,0);
	    cvKalmanCorrect( kalman, kmes );
	    line(frame, Point(frame.cols/2,0), Point(150*cos(cpolar.y*PI/180)+150, 150*sin(cpolar.y*PI/180)), red, 1, 8, 0);
	    line(frame, Point(frame.cols/2,0), Point(150*cos(value*PI/180)+150, 150*sin(value*PI/180)), blue, 1, 8, 0);
	    

            //imshow("kalman", biggest_contour);
            //imshow("frame", frame);
        }
        else 
        {
	    //cout << endl << " no contour";
        }
	
	//decision making for target state

	if ( k > 74 ) { k = 0; }	//movement in the last 5 seconds
	avrg[k] = cpolar.y;
	++k;
	avg = 0;
	for ( j=0;j<75;++j){ avg += avrg[j]; }
	
	if ( cpolar.y < 5 || cpolar.y > 175 ) //if target is out of view (teta < 5 or teta > 175)
	{
	    if( wit > 300 ) {state = 0;} //wait about a min before going to 0
	    else { ++wit; } 
	}
	else //if target is in view (5 < teta < 175)
	{
	    if ( cpolar.y == avg/75 ) //if target is not moving for 5 sec
	    {
		if ( wit > 300 ) {state = 0;} //wait about a min before going to 0
		else { ++wit;}
	    }
	    else { state = 1; wit = 0;} //if target is moving in the view area record and set the wait to 0
	}
	    
	    //cout << endl << " x = " << last_center.x;
	    //cout << " y = " << last_center.y;
	    //cout << " wait = " << wit;
	    //cout << " avg = " << avg/75;
	    //cout << endl << " r = " << cpolar.x;
	    cout << " (teta,state) = " << "(" << cpolar.y << "," << state << ")" << endl;
	    //cout << endl << " kalman out = " << value;
	    //cout << endl << " biggest contour size = " << contours[larg_contour_index].size();
	    //cout << endl << " xp = " << cos(cpolar.y*PI/180);
	    //cout << " yp = " << sin(cpolar.y*PI/180);
	    //cout << endl << " cos(v) = " << cos(value*PI/180);
	    //cout << " sin(v) = " << sin(value*PI/180);

//        cout <<endl<<endl;
        ++frame_count;  

        //if( waitKey(1) >= 0) break;
    }
    return 0;
}
