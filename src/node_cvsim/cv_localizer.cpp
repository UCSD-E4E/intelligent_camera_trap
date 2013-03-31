#include "ICT_Viper/CvService.h"

class CVLocalizer
{
		int x, y, imgWidth, imgHeight; 
		double timestamp;
	public:
		CVLocalizer(int x_init, int y_init, int imgW, int imgH);
		void updateLocation(int updated_x, int updated_y)
		{
			x = updated_x;
			y = updated_y;
		};

		int getX(void)
		{
			return x;
		}

		int getY(void)
		{
			return y;
		}

		double getTimsestamp(void)
		{
			return Timestamp();
		}

		void setTimestamp(double new_timestamp)
		{
			timestamp = new_timestamp;
		}

		bool newCoords(ICT_Viper::CvService::Request &req, ICT_Viper::CvService::Response &res); 	


};

CVLocalizer(int x_init, int y_init, int imgW, int imgH)
{
	x = x_init;
	y = y_init;

	imgWidth = imgW;
	imgHeight = imgH;

	timestamp = 0;
}

bool newCoords(ICT_Viper::CvService::Request &req, ICT_Viper::CvService::Response &res)
{
	if (req.A == 0)
	{
		ROS_INFO("Recieved CV localization request\n");
		rex.Coords = x;
	}
	return true;
}


