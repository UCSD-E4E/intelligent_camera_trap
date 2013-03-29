#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <iostream>
#include <time.h>
#include "utilities.h"
#include <sys/statvfs.h>
//#include "flirROS.h"
//using namespace std;

int send_motor = 0;
bool vibration_trigger = false;
<<<<<<< HEAD
long int getDiskSpace();

=======


int Wait_For_Trigger(void);
int Track_Object(void);
int Extract_Files(void);


long int getDiskSpace();


>>>>>>> 220515bba5f0e3fa999186b88b8df98e380487df
void set_motor(){
	send_motor = 1;
}

void VibrationCallback(const std_msgs::String::ConstPtr& msg){
	ROS_INFO("Vibration callback recieved: [%s]", msg->data.c_str());
	vibration_trigger = true;

	set_motor();
	//trackObject();
}

<<<<<<< HEAD
=======

>>>>>>> 220515bba5f0e3fa999186b88b8df98e380487df
long int getDiskSpace(){
	struct statvfs vfs;
	statvfs("/home/", &vfs);
	unsigned long free = vfs.f_bavail;
	unsigned long total = vfs.f_blocks;
	return ((free*100)/total);
}


int main(int argc, char **argv){	
	char mtrBuf[10];
	std_msgs::String msg;

	ros::init(argc, argv, "main");
	ros::NodeHandle n;

	ROS_INFO("disk space percent free: %ld", getDiskSpace());
	
<<<<<<< HEAD
=======
	
	
>>>>>>> 220515bba5f0e3fa999186b88b8df98e380487df
	ROS_INFO("attempting to subscribe to Vibration_Node");
	ros::Subscriber sub = n.subscribe("Vibration_Node", 10, VibrationCallback);	
	ros::Publisher pan = n.advertise<std_msgs::String>("Motor_Instructions", 10);	 
	while(ros::ok()){
		if  (send_motor){
			ROS_INFO("Sending Motor Command...");
			sprintf(mtrBuf, "turn");
			msg.data = mtrBuf;
			pan.publish(msg);
			send_motor = 0; 
		}
		ros::spinOnce();	
	}
	return 0;
} 
	
