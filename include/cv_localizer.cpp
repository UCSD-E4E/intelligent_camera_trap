#include "CamTrap_Viper/CvService.h"
#include "cv_localizer.h"
#define PI 3.14159

void CVLocalizer::updateLocation(int updated_x, int updated_y)
{
	x = updated_x;
	y = updated_y;
};

void CVLocalizer::setX(int new_x)
{
	x = new_x;
}

void CVLocalizer::setY(int new_y)
{
	y = new_y;
}

double CVLocalizer::getTimsestamp(void)
{
	return timestamp;
}

void CVLocalizer::setTimestamp(double new_timestamp)
{
	timestamp = new_timestamp;
}


CVLocalizer::CVLocalizer(int x_init, int y_init, const int imgW, const int imgH, const int FOV_X, const int FOV_Y)
{
	x = x_init;
	y = y_init;

	imgWidth = imgW;
	imgHeight = imgH;
	
	offset_const_x = tan((PI/180)*(FOV_X/2))/(imgW/2); 
	offset_const_y = tan((PI/180)*(FOV_Y/2))/(imgH/2);
	//ROS_INFO("offset_const_x = %f", offset_const_x);
	timestamp = 0;
}

bool CVLocalizer::newCoords(CamTrap_Viper::CvService::Request &req, CamTrap_Viper::CvService::Response &res)
{
	if (req.localization_request == 0)
	{
	//	ROS_INFO("Recieved CV localization request\n");
		res.x_offset = x;
		res.x_degree = (180/(PI))*atan2((x*offset_const_x),1);
		
		res.y_offset = y;
		res.y_degree = (180/(PI))*atan2((y*offset_const_y),1);
	}
	return true;
}


