#ifndef _STATE_H_
#define _STATE_H_

#define STATE_INIT                 0
#define STATE_TURN_CAMERA_ON       1
#define STATE_TRACKING             2
#define STATE_TURN_CAMERA_OFF      3
#define STATE_SHUTTING_DOWN        4

#define TIME_FOR_STATE_TURN_CAMERA_ON_S      (3 * 8)
#define TIME_OUT_FOR_NOT_TRACKING            (5 * 8)
#define TIME_FOR_STATE_TURN_CAMERA_OFF_S     (3 * 8)

#endif