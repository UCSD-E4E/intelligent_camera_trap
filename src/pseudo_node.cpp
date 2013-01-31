#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

int main(int argc, char **argv){
	ros::init(argc, argv, "pseudo_node");
	ros::NodeHandle n;
	ros::Publisher vibration = n.advertise<std_msgs::String>("pseudo_node", 10);
	
	ros::Rate loop_rate(1);
	
	int count = 0;
	while(ros::ok()){
		std_msgs::String msg;
		std::stringstream ss;
		ss << "fake trigger " << count;
		msg.data = ss.str();

		ROS_INFO("%s", msg.data.c_str());
		vibration.publish(msg);

	loop_rate.sleep();
	count++;
	}
	return 0;
}
