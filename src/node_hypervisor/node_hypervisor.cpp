#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <iostream>
#include <time.h>




int main(int argc, char **argv)
{
	//Initialize ROS node setup
	ros::init(argc, argv, "hypervisor");
	ros::NodeHandle n;

	ros::Publisher pub = n.advertise<std_msgs::String>("Hypervisor_Output",10);
	
	while(ros::ok)
	{
		//Do stuff until someone presses ctrl-c
	}
	return 0;
}
