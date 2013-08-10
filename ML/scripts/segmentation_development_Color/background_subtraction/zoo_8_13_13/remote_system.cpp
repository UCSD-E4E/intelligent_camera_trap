/*
 * Run the openCV background subtraction MOG2 on input frames
 * from a webcam.  Find the center of mass for images segmented
 * this way, move turret accordingly.
 * Save frames from the webcam making a new folder every 5 minutes
 * in the directory specified in "image_file_directory"
 */



#include<opencv2/opencv.hpp>
#include<iostream>
#include <vector>
#include <time.h>
#include <string>
#include <cstdlib>
using namespace cv;
using namespace std;


/*
 * Return the centroid of a segmented binary image.
 */
Point getCentroid(cv::Mat img)
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
	Mat frame;
    Mat gray;
	Mat fore;

    string image_file_directory ("/home/sealab/Desktop/");
    string image_file_name;
    string make_directory;
    string current_directory;
    string current_frame;
    const char* make_directory_c;
    
    BackgroundSubtractorMOG2 bg;
    Point last_center;
    unsigned long frame_count = 0;
    int x = 0;
    int y = 0;
	
    char current_time[24];
    char next_time_c[24];
    time_t rawtime;
    time_t start_time;
    time_t next_time;

    VideoCapture cap(0);
	if(!cap.isOpened())
	{
        cout << endl;
		cout << "Failed to connect to the 360 camera." <<endl;
        cout << endl;	
    }
	
    else
	{
        cout << endl;
		cout<< "Connected to 360 camera." << endl;
        cout << endl;	
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 160);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 120);


    //make first image directory
    time(&start_time);
    strftime(current_time, 24, "%m-%d-%H:%M:%S",localtime(&start_time));
    make_directory = "mkdir " + image_file_directory + current_time;
    current_directory = image_file_directory + current_time + "/";    
    make_directory_c = make_directory.c_str();
    system(make_directory_c);
    
    next_time = start_time + 300; //next directory should be 5 mins later
	
	for(;;)
	{
		cap >> frame;
        
        time(&rawtime);
        strftime(current_time, 24, "%m-%d-%H:%M:%S",localtime(&rawtime));

        //make a new directory every 5 mins        
        if( rawtime >= next_time )
        {
            strftime(next_time_c, 24, "%m-%d-%H:%M:%S",localtime(&next_time));
            make_directory = "mkdir " + image_file_directory + next_time_c;
            current_directory = image_file_directory + next_time_c +"/";            
            make_directory_c = make_directory.c_str();
            system(make_directory_c);

            next_time += 300;  //next directory 5 mins after this one
            frame_count = 0;   //reset frame_count
        }
        
        cvtColor(frame, gray, CV_BGR2GRAY); //grayscale the frame
		bg.operator()(gray,fore);           //get the binary foreground image
        last_center = getCentroid(fore);    
        x += last_center.x;
        y += last_center.y;
        
        if (frame_count % 10 == 0)          //every 10 frames, come up w/ a new tracking location
        {
            x = int(x/10.);  //avg all our centroid coordinates
            y = int(y/10.);
            last_center.x = x;
            last_center.y = y;
            
            //MOTOR CONTROL HERE

            x = 0;
            y = 0;
        }
            
        current_frame = to_string(frame_count);
        image_file_name = current_directory + current_time + "_" + current_frame + ".jpg";
        imwrite(image_file_name, frame);

        ++frame_count;  

        if( waitKey(5) >= 0) break;
	}
	return 0;
}
