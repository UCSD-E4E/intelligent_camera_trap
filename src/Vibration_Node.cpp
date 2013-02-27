#include </opt/ros/fuerte/include/ros/ros.h>
#include </opt/ros/fuerte/include/ros/duration.h>
#include </opt/ros/fuerte/include/std_msgs/String.h>
#include <sstream>

#include <fcntl.h>
#include <termios.h>
//#include <iostream.h>
#include <string.h>


int main(int argc, char** argv){
	ros::init(argc, argv, "Vibration_Node");
	ros::NodeHandle n;
	//Advertise Vibration Service, sends an integer corresponding to a sensor node trigger 
	ros::Publisher vibration = n.advertise<std_msgs::String>("Vibration_Node", 0);
	
	int fd, i;
	ssize_t rd;
	char buffer[5] = {0,0,0,0,0};
	char cmpBuf[2];
	int node_id = -1;
	char msgBuf[3];
	std_msgs::String msg;
		
	//fd = open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_NDELAY);
	ROS_INFO("fd = %d", fd);
		
	while (ros::ok()){
		fd = open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_NDELAY);
	//	ROS_INFO("fd = %d", fd);
		fcntl(fd, F_SETFL, 0);
		rd = read(fd, buffer, 4);
		if (rd != 0){
			ROS_INFO("rd = %ld", rd);
		}
	
		
		if(buffer[0]=='t'){
			ROS_INFO("buffer = %s", buffer);
			//parse node number as an integer
			node_id = buffer[3]- 48;
			ROS_INFO("node number %d", node_id);
			if (node_id > 0 && node_id < 10){
				//we have a valid node id parse
				ROS_INFO("Node trigger on node %d", node_id);
				sprintf(msgBuf, "%d", node_id);
				msg.data = msgBuf;
				vibration.publish(msg); 
				ros::Duration(5).sleep();	
			}
			else{
				ROS_INFO("Node ID Parse Error");
			}	
		}
		for (i=0;i<5;i++)
		{
			buffer[i] = 0;
		}
		close(fd);
	}  
} 
