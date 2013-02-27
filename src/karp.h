#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

int i = 0;
int fd = 0;

int openSerial(void);
char* degrees_to_serial(char movebuff[8], char cmd[8], double degrees);

/****Serial Command List KARPPPP***/
unsigned char stop_cmd[4] = {'Q','U','I', 'T'};
unsigned char start_cmd[4] = {'S', 'T', 'R', 'T'};
char pan_cmd[6] = {'X','N',' ','0', '0', '0'};
char tilt_cmd[6] = {'Z','N',' ','0', '0', '0'};
char turn_turret[8] = {'X', '0', '0', '0', 'Z', '0', '0', '0'}; 
unsigned char release_camera[4] = {'R','E','S','T'};
unsigned char wake_camera[4] = {'W','A','K','E'};
/**********************************/

/*****************************************************/
char* degrees_to_serial(char movebuff[8], char cmd[8], double degrees_x,  double degrees_z)
{
	movebuff = cmd;
	/*for (i = 0;i<8;i++)
	{
		printf("%d", movebuff[i]);
	}
	printf("\n");*/
	int x = (int)degrees_x;
	int z = (int)degrees_z;
	
	//printf("Integer degrees: %d\n", turn_degrees);
	
	if (x < 0)
	{
		movebuff[1] = '-';
		x*=-1;
	}
	else
		movebuff[1] = '+';
	
	if (z < 0)
	{
		movebuff[5] = '-';
		z*=-1;
	}
	else
		movebuff[5] = '+';
	
	int n;
	for (n = 3; n > 1; n--)
	{
		movebuff[n] = x%10 + 48;
		x /= 10;
	}
	for (n = 7; n > 5; n--)
	{
		movebuff[n] = z%10 + 48;
		z /= 10;
	}
	return movebuff;
}
/********************************************************/

int openSerial(void)
{
	
	fd = open("/dev/ttyUSB0", O_RDWR | O_NDELAY | O_NONBLOCK);
	
	if (fd == -1)
	{
		printf("Failed to open serial port\n");		
		return -1;
	}
	fcntl(fd, F_SETFL, 0);
	printf("fd = %d\n", fd);

	struct termios options;

	tcgetattr(fd, &options);

	memset(&options,0,sizeof(options));
        options.c_iflag=0;
        options.c_oflag=0;
        options.c_cflag=CS8|CREAD|CLOCAL;           
        options.c_lflag=0;
        options.c_cc[VMIN]=1;
        options.c_cc[VTIME]=5;

	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);

	tcsetattr(fd, TCSANOW, &options);
	return 0;
}

