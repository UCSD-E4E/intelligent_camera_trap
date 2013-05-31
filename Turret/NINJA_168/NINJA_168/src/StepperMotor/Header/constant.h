#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#define TRUE  1
#define FALSE 0

#define ON    1
#define OFF   0

#define SIDE_UNDEFINED 0
#define SIDE_LEFT      1
#define SIDE_RIGHT     2

#define LEFT_CH_1  0
#define LEFT_CH_2  1
#define LEFT_CH_3  2
#define LEFT_CH_4  3

#define RIGHT_CH_1 LEFT_CH_4
#define RIGHT_CH_2 LEFT_CH_3
#define RIGHT_CH_3 LEFT_CH_2
#define RIGHT_CH_4 LEFT_CH_1

#define NO_CHANNEL       0
#define LEFT_CH_STEP_1   1
#define LEFT_CH_STEP_2   2
#define LEFT_CH_STEP_3   4
#define LEFT_CH_STEP_4   8

#define RIGHT_CH_STEP_1  LEFT_CH_STEP_4
#define RIGHT_CH_STEP_2  LEFT_CH_STEP_3
#define RIGHT_CH_STEP_3  LEFT_CH_STEP_2
#define RIGHT_CH_STEP_4  LEFT_CH_STEP_1

#define LEFT_LIMIT 0
#define RIGHT_LIMIT 1000
#define CENTER_PAN (RIGHT_LIMIT/2)

#define POSITION_LEFT       0
#define POSITION_RIGHT      1
#define POSITION_CENTER_PAN 2
#define POSITION_UNDEFINED  3

#define STAY_PUT   0
#define MOVING_LEFT  1
#define MOVING_RIGHT 2
#define MOVING_LEFT_ACC 3
#define MOVING_RIGHT_ACC 4

#define PAN_FASTEST_QUARTER_STEP_IN_2MS 1
#define PAN_SLOWEST_QUARTER_STEP_IN_2MS 4

#define SPEED_CONSTANT   0
#define SPEED_ACCELERATE 1
#define SPEED_SLOWEST    2

#define SIDE_UP        0
#define SIDE_DOWN      1
#define SIDE_UNDEFINED 2

#define UP_CH_1  0
#define UP_CH_2  1
#define UP_CH_3  2
#define UP_CH_4  3

#define DOWN_CH_1 UP_CH_4
#define DOWN_CH_2 UP_CH_3
#define DOWN_CH_3 UP_CH_2
#define DOWN_CH_4 UP_CH_1

#define UP_CH_STEP_1   1
#define UP_CH_STEP_2   2
#define UP_CH_STEP_3   4
#define UP_CH_STEP_4   8

#define DOWN_CH_STEP_1  UP_CH_STEP_4
#define DOWN_CH_STEP_2  UP_CH_STEP_3
#define DOWN_CH_STEP_3  UP_CH_STEP_2
#define DOWN_CH_STEP_4  UP_CH_STEP_1

#define DOWN_LIMIT 0
#define CENTER_TILT (UP_LIMIT/2)
#define UP_LIMIT 400

#define POSITION_DOWN        0
#define POSITION_UP          1
#define POSITION_CENTER_TILT 2
#define POSITION_UNDEFINED   3

#define STAY_PUT        0
#define MOVING_DOWN     1
#define MOVING_UP       2
#define MOVING_DOWN_ACC 3
#define MOVING_UP_ACC   4

#define TILT_FASTEST_QUARTER_STEP_IN_2MS 6
#define TILT_SLOWEST_QUARTER_STEP_IN_2MS 12

#endif
