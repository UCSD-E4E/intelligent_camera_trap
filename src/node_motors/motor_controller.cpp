#include "motor_controller.h"
#define TOLERANCE 0.0
#define PAN_STEPS 883.0
#define PAN_RANGE 180.0
#define TILT_STEPS 355.0
#define TILT_RANGE 72.0

using namespace boost::asio;

void MotorController::updatePosition()
{
	//update current motor position
	updatePanTilt();
	if ((abs(new_pan - pan_pos) > TOLERANCE) || (abs(new_tilt - tilt_pos) > TOLERANCE))
	{
		int pan_steps = (new_pan - pan_pos)*PAN_STEPS/PAN_RANGE; 
		sendRelSteps(pan_steps, 0);
	}
	else
	{
		ROS_INFO("Change not big enough to move motors");
	}
	return;
}

void MotorController::updatePanTilt()
{
	//update pan/tilt position: get steps
 	char query[7];
 	sprintf(query, "STQ%c9A", 1);

	writeString(query);
	readCoords();
	return;
}

void MotorController::readCoords()
{
	using namespace boost;
	system::error_code ec;
	int min_length = 24;	
	char c[64];
	int LR, UD;
	LR = UD = -10;
	int i;
	//Give message a chance to transfer	
//	ros::Duration(0.5).sleep();
	
	asio::read(serial, asio::buffer(c), asio::transfer_at_least(min_length), ec);

	if (ec)
	{	
		ROS_ERROR("Boost Error at readCoords()");
	}

	ROS_INFO("From Controller: %s", c);

	int coord_len;
	for (coord_len = 0; coord_len < min_length; coord_len++)
	{
		if ((c[coord_len] == '\n') && (coord_len > 7))
			break;
	} 	
	ROS_INFO("Read %d chars before newline", coord_len);	
	char* filtered = (char *) malloc(coord_len);
	
	for (i = 0; i < coord_len; i++)
	{
		filtered[i] = c[i];
	}	

	ROS_INFO("filtered input: %s", filtered);
	int x_steps, y_steps;
	sscanf(filtered, "X:%d Y:%d", &x_steps, &y_steps);

	if (x_steps > 883)
	{
		ROS_INFO("Confirmation read failure");
		return;
	}

	pan_pos = x_steps*(PAN_RANGE/PAN_STEPS);
	tilt_pos = y_steps*(TILT_RANGE/TILT_STEPS);
	ROS_INFO("Current Position: [%f, %f]", pan_pos, tilt_pos);	
	return;	
}

void MotorController::readResponse()
{
	using namespace boost;
	system::error_code ec;
	int min_length = 18;
	char c[64];

//	ros::Duration(0.1).sleep();
	
	asio::read(serial, asio::buffer(c), asio::transfer_at_least(min_length), ec);
	if (ec)
	{
		ROS_ERROR("Boost Error at readResponse");
	}

	ROS_INFO("From Controller: %s", c);

	int x_conf, y_conf;
	sscanf(c, "Target X:%d Y:%d", &x_conf, &y_conf);

	ROS_INFO("Confirmation: [%d, %d]", x_conf, y_conf);
	return;
	
}

void MotorController::writeString(char* s)
{
	boost::asio::write(serial, boost::asio::buffer(s, strlen(s)));
	ROS_INFO("wrote %s to /dev/ttyUSB0", s);
}

int MotorController::sendSteps(int steps_x, int steps_y)
{
	//construct message
//	unsigned char checksum = 'A';
	char* packet = (char *)malloc(snprintf(NULL, 0, "X0%dY0%d", steps_x, steps_y) + 1);
	char char_count = sprintf(packet, "X0%dY0%d", steps_x, steps_y);
	
	char* cmd = (char *)malloc(snprintf(NULL, 0, "STD%c%sA", char_count, packet) + 1);
	sprintf(cmd, "STC%c%sA", char_count, packet);
	
	writeString(cmd);

	//check confirmation
	ROS_INFO("Steps = [%d, %d]", steps_x, steps_y); 
	return 0;
}

int MotorController::sendRelSteps(int steps_x, int steps_y)
{
	char dir_x, dir_y;
	//Calculate direction code
	if (steps_x >= 0)
	{ 
		dir_x = 'R';
	}
	else
	{
		dir_x = 'L';
		steps_x*=-1;
	}
	
	if (steps_y >= 0)
	{
		dir_y = 'U';
	}
	else
	{
		dir_y = 'D';
		steps_y*=-1;
	}

	char* packet = (char *)malloc(snprintf(NULL, 0, "%c0%d%c0%d", dir_x, steps_x, dir_y, steps_y) + 1);
	char char_count = sprintf(packet, "%c0%d%c0%d", dir_x, steps_x, dir_y, steps_y);
	char* cmd = (char *)malloc(snprintf(NULL, 0, "STD%c%sA", char_count, packet) + 1);

	sprintf(cmd, "STD%c%sA", char_count, packet);

	writeString(cmd);
	readResponse();
	ROS_INFO("Relative Steps: [%d, %d]", steps_x, steps_y);
	
	return 0;

}

	
std::string MotorController::readPort()
{
	ROS_INFO("Reading port...");
	using namespace boost;
	char c;
	std::string result;
	for(;;)
	{
		asio::read(serial, asio::buffer(&c, 1));
		
		switch(c)
		{
			case '\r':
				break;
			case '\n':
				return result;
			default:
				result+=c;
		}
	}
}
