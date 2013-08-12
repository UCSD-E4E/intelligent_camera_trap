#include "motor_controller.h"
#define TOLERANCE 0.0
#define PAN_STEPS 883.0
#define PAN_RANGE 180.0

#define TILT_STEPS 355.0
#define TILT_RANGE 72.0

using namespace boost::asio;

//int oneNorm(int* x, int* y);
//int oneNorm(int* x, int* y)
//{
//	return abs(*x - *y) + abs(*(x+1) - *(y+1));
//}
void MotorController::updatePosition()
{
	//update current motor position
	updatePanTilt();
	if ((abs(new_pan - pan_pos) > TOLERANCE) || (abs(new_tilt - tilt_pos) > TOLERANCE))
	{
		int pan_steps = (new_pan - pan_pos)*PAN_STEPS/PAN_RANGE;
		int tilt_steps = (new_tilt - tilt_pos)*TILT_STEPS/TILT_RANGE;
		sendRelSteps(pan_steps, tilt_steps); 
	}
	else
	{
		printf("Change not big enough to move motors");
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
	char b;
	int LR, UD;
	LR = UD = -10;
	int i;
	int x_steps, y_steps;
	int read_coords;
	int read_lrud;
	int read_count = 0;
	read_coords = read_lrud = 0;
	
	//read both inputs:
	while (!(read_coords && read_lrud) && read_count < 3)
	{
		//read until newline
		for (i=0;i<64;i++)
		{
			asio::read(serial, asio::buffer(&b,1));
			if (b == '\n')
				break;
			else
				c[i] = b;
		}	
	
		//pattern matching
		if ((c[0] == 'L') || (c[1] == 'R'))
		{
			printf("LRUD indicator: %s", c);
			sscanf(c, "LR:%d UD:%d", &LR, &UD); 
			printf("[LR, UD] = [%d,%d]", LR, UD);
			read_lrud = 1;
		}
		else if ((c[0] == 'X') || (c[1] == ':'))
		{
			printf("Offset message: %s", c);
			sscanf(c, "X:%d Y:%d", &x_steps, &y_steps);
			pan_pos = x_steps*(PAN_RANGE/PAN_STEPS);
			tilt_pos = y_steps*(TILT_RANGE/TILT_STEPS);
			printf("Current Position: [%f, %f]", pan_pos, tilt_pos);	
			read_coords = 1;
		}
		else
		{
			printf("read error, input: %s", c);
		}
	
		//clean out c buffer for future use
		for (i = 0; i < 64; i++)
		{
			c[i] = '\0';
		} 	
		read_count++;
	}
	return;	
}

void MotorController::readResponse()
{
	using namespace boost;
	int min_length = 24;
	char c[64];
	char b;
	int readRes = 0;
	int x_conf, y_conf, i;
	

	while(!readRes)
	{	
		//read line
		for (i=0;i<64;i++)
		{
			asio::read(serial, asio::buffer(&b,1));
			if (b == '\n')
				break;
			else
				c[i] = b;
		}
		
		//pattern match		
		if ((c[0] == 'T') || (c[1] == 'a'))
		{
			sscanf(c, "Target T1:%d T2:%d", &x_conf, &y_conf);
			printf("Confirmation: [%d, %d]", x_conf, y_conf);
			readRes = 1;
		}
		
		printf("From Controller: %s", c);	
		
		//clean buffer
		for (i=0;i<64;i++)
		{
			c[i] = '\n';	
		}
	}
 
	return;
	
}


void MotorController::writeString(char* s)
{
	boost::asio::write(serial, boost::asio::buffer(s, strlen(s)));
	printf("wrote %s to /dev/ttyUSB0", s);
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
	printf("Steps = [%d, %d]", steps_x, steps_y); 
	return 0;
}

int MotorController::sendRelSteps(int steps_x, int steps_y)
{
	//steps_x *= 0.9;
	//steps_y *= 0.9;
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

	char* packet = (char *)malloc(snprintf(NULL, 0, "%c0%03d%c0%03d", dir_x, steps_x, dir_y, steps_y) + 1);
	int char_count = sprintf(packet, "%c0%03d%c0%03d", dir_x, steps_x, dir_y, steps_y);
	char* cmd = (char *)malloc(snprintf(NULL, 0, "STD%c%sA", (char)char_count, packet) + 1);

	sprintf(cmd, "STD%c%sA", char_count, packet);

	writeString(cmd);
	readResponse();
	printf("Relative Steps: [%d, %d]", steps_x, steps_y);
	
	return 0;

}

	
std::string MotorController::readPort()
{
	printf("Reading port...");
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
