#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <iostream>

#include <ros/ros.h>

#include "ICT_Viper/CvService.h"
#include "parameters.h"


int main(int argc, char **argv)
{
	ros::init(argc, argv, "cv_service");
	ros::NodeHandle n;

	CVLocalizer object_tracker(240, 320, 480, 640);
	object_tracker.setTimestamp(0);


	ros::ServiceServer service = n.advertiseService("cv_service", &CVLocalizer::newCoords, &object_tracker);
	
	
	double start = ros::Time::now().toSec();
	double current_time = start;
	delta = 10;
	while(ros::ok())
	{
		current_time = ros::Time::now().toSec();
		x += delta;
		if (object_tracker.x >= 640)
		{
			object_tracker.x = 640;
			delta *= -1;
		}
		if (object_tracker.x <= 0)
		{
			object_tracker.x = 0;
			delta *= -1;
		}

		object_tracker.setTimestamp(current_time - start);
		ros::Duration(1).sleep();
	}

	return 0;
}


