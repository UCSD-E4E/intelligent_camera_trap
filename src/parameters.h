#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

#define V_AVGFRAMES 3
#define CAMERA_FOV 53.0

#define START_HOUR 8
#define START_MINUTE 0
#define STOP_HOUR 20
#define STOP_MINUTE 0

#define WEBCAM 0 //corresponds to /dev/video[WEBCAM]

#define X_OFFSET_TOLERANCE 10
#define Y_OFFSET_TOLERANCE 7

#define USE_KALMAN 0	//0 = raw offset; 1 = apply kalman filter

#define SHRINK_PERCENT 100.00		//int 1 to 100: shrinks video to SHRINK_PERCENT of the original size

#define MINIMUM_DISK_SPACE 5	
/*integer from 0 to 100, defines when the system will terminate due to lack of storage space. When the free space on the storage disk drops below this level, the system will deactivate*/

#define INTERESTING_THRESHOLD 10

#define BLOB_THRESHOLD 300
