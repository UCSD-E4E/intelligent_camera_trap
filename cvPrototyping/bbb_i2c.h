/***********************
bbb_i2c.h
***********************/
#include <stdint.h>

int I2C_OPEN(unsigned char bus, unsigned char addr);

int I2C_WRITE(int handle, unsigned char* buf, unsigned int length);

int I2C_WRITE_BYTE(int handle, unsigned char val);

int I2C_READ(int handle, unsigned char* buf, unsigned int length);

int I2C_WRITE_READ(int handle, unsigned char addr_w, unsigned char *buf_w, unsigned int len_w, unsigned char addr_r, unsigned char *buf_r, unsigned int len_r);
				   
int I2C_CLOSE(int handle);

int WAIT(unsigned int msec);

int INITIALIZE(void);

void NORMALIZE(float (*array)[16]);

void frame(uint8_t (*pIRTemp)[16]);

