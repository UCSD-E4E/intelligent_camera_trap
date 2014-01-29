#ifndef bbb_hearbeat_H_
#define bbb_hearbeat_H_

unsigned char get_bbb_heart_beat(void);
void set_bbb_heart_beat(unsigned char currentHearBeat);
unsigned int get_bbb_heart_beat_inactive_time_in_S(void);
void increment_bbb_heart_beat_inactive_time_100MS(void);
extern unsigned char readBBBHeartBeatLine(void);

#endif
