#include "CamTrap_Viper/CvService.h"
#include "cv_localizer.h"


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


CVLocalizer::CVLocalizer(int x_init, int y_init, int imgW, int imgH)
{
	x = x_init;
	y = y_init;

	imgWidth = imgW;
	imgHeight = imgH;

	timestamp = 0;
}

bool CVLocalizer::newCoords(CamTrap_Viper::CvService::Request &req, CamTrap_Viper::CvService::Response &res)
{
	if (req.localization_request == 0)
	{
	//	ROS_INFO("Recieved CV localization request\n");
		res.localization[0] = x;
	}
	return true;
}


