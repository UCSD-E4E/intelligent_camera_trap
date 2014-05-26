/***********************************************************
 * Program to access Melexis MLX90620 sensor from BBB
 ***********************************************************/

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdio.h> 
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "bbb_i2c.h"

// I2C2 BUS 1 on BBB
#define I2C_BUS 1

/**
  \def MLX620_IR_ROWS
  Number of rows in IR array.
*/
#define MLX620_IR_ROWS (4U)
/**
  \def MLX620_IR_COLUMNS
  Number of columns in IR array.
*/
#define MLX620_IR_COLUMNS (16U)

/**
  \def MLX620_IR_SENSORS
  Number of sensors in IR array.
*/
#define MLX620_IR_SENSORS (MLX620_IR_ROWS * MLX620_IR_COLUMNS)


unsigned char EE_BUFFER[256];
short int IRTemp_raw[MLX620_IR_SENSORS];


int I2C_OPEN(unsigned char bus, unsigned char addr)
{
  int file;
  char filename[16];
  sprintf(filename,"/dev/i2c-%d", bus);
  if ((file = open(filename,O_RDWR)) < 0)
  {
    fprintf(stderr, "I2C_OPEN open error: %s\n", strerror(errno));
    return(file);
  }
  if (ioctl(file,I2C_SLAVE,addr) < 0)
  {
    fprintf(stderr, "I2C_OPEN ioctl error: %s\n", strerror(errno));
    return(-1);
  }
  return(file);
}

int I2C_WRITE(int handle, unsigned char* buf, unsigned int length)
{
  if (write(handle, buf, length) != length)
  {
    fprintf(stderr, "I2C_WRITE error: %s\n", strerror(errno));
    return(-1);
  }
  return(length);
}

int I2C_WRITE_BYTE(int handle, unsigned char val)
{
  if (write(handle, &val, 1) != 1)
  {
    fprintf(stderr, "I2C_WRITE_BYTE error: %s\n", strerror(errno));
    return(-1);
  }
  return(1);
}

int I2C_READ(int handle, unsigned char* buf, unsigned int length)
{
  if (read(handle, buf, length) != length)
  {
    fprintf(stderr, "I2C_READ error: %s\n", strerror(errno));
    return(-1);
  }
  return(length);
}

int I2C_WRITE_READ(int handle, unsigned char addr_w, unsigned char *buf_w, unsigned int len_w,
                   unsigned char addr_r, unsigned char *buf_r, unsigned int len_r)
{
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg msgs[2];
	
	msgs[0].addr=addr_w;
	msgs[0].len=len_w;
	msgs[0].flags=0;
	msgs[0].buf=buf_w;
	
	msgs[1].addr=addr_r;
	msgs[1].len=len_r;
	msgs[1].flags=1;
	msgs[1].buf=buf_r;
	
	msgset.nmsgs=2;
	msgset.msgs=msgs;
	
	if (ioctl(handle,I2C_RDWR,(unsigned long)&msgset)<0)
  {
		fprintf(stderr, "I2C_WRITE_READ error: %s\n",strerror(errno));
    return -1;
  } 
  return(len_r);
}

int I2C_CLOSE(int handle)
{
  if ((close(handle)) != 0)
  {
    fprintf(stderr, "I2C_CLOSE error: %s\n", strerror(errno));
    return(-1);
  }
  return(0);
}

int WAIT(unsigned int msec)
{
  int ret;
  struct timespec a;
  if (msec>999)
  {
    fprintf(stderr, "WAIT error: delay value needs to be less than 999\n");
    msec=999;
  }
  a.tv_nsec=((long)(msec))*1E6d;
  a.tv_sec=0;
  if ((ret = nanosleep(&a, NULL)) != 0)
  {
    fprintf(stderr, "WAIT error: %s\n", strerror(errno));
  }
  return(0);
}

int INITIALIZE(void)
{
	int handle;
	unsigned char buf[10];
  //unsigned char EE_BUFFER[256];
  unsigned char buf_r[10];
	
  WAIT(10);
  
  handle=I2C_OPEN(I2C_BUS, 0x50);
  I2C_WRITE_BYTE(handle, 0x00);
  I2C_READ(handle, EE_BUFFER, 256);
  I2C_CLOSE(handle);
 
  // oscillator trimming value
  buf[0]=0x04; // command
  buf[1]=EE_BUFFER[0xf7]-0xaa;
  buf[2]=EE_BUFFER[0xf7];
  buf[3]=0x56;
  buf[4]=0x00;
  handle=I2C_OPEN(I2C_BUS, 0x60);
  I2C_WRITE(handle, buf, 5);
  
   // configuration register
  buf[0]=0x03; // command
  buf[1]=EE_BUFFER[0xf5]-0x55;
  buf[2]=EE_BUFFER[0xf5];
  EE_BUFFER[0xf6]=EE_BUFFER[0xf6] | 0x04; //ensure bit 10 is set always
  buf[3]=EE_BUFFER[0xf6]-0x55;
  buf[4]=EE_BUFFER[0xf6];
  I2C_WRITE(handle, buf, 5);
  
  
  
  I2C_CLOSE(handle);
  handle=I2C_OPEN(I2C_BUS, 0x60);
 
  
  // read the configuration register to check the POR bit
  buf[0]=0x02; // command
  buf[1]=0x92; // start address
  buf[2]=0x00; // address step
  buf[3]=0x01; // number of reads
  I2C_WRITE_READ(handle,0x60, buf, 4, 0x60, buf_r, 2);
  I2C_CLOSE(handle);
  
  if((buf_r[1] & 0x04)==0)
  {
  	// error
  	printf("config reg is incorrect!\n");
  	return(-1);
  }
  
  return(0);
}

unsigned int ptat_read(void)
{
	int handle;
	unsigned int ptat;
	unsigned char buf[4];
	unsigned char buf_r[2];
	
    handle=I2C_OPEN(I2C_BUS, 0x60);
  buf[0]=0x02; // command
  buf[1]=0x90; // start address
  buf[2]=0x00; // address step
  buf[3]=0x01; // number of reads
  
  I2C_WRITE_READ(handle,0x60, buf, 4, 0x60, buf_r, 2);
  
  I2C_CLOSE(handle);
   
  ptat=(((unsigned int)buf_r[1])<<8) | ((unsigned int)buf_r[0]);
  
  return(ptat);
}
  
void calc_ta_coeff(float* vth25, float* kt1, float* kt2)
{
	typedef struct ta_param_s
	{
		short int ee_vth;
		short int ee_kt1;
		short int ee_kt2;
	} ta_param_t;
	
	ta_param_t ee;
	
	memcpy((void*)&ee, (void*)&EE_BUFFER[0xda], 6);
		
	*vth25=(float)ee.ee_vth;
	*kt1=((float)ee.ee_kt1)/1024;
	*kt2=((float)ee.ee_kt2)/1048576;
}

float calc_ta(unsigned int ptat, float vth25, float kt1, float kt2)
{
	float ta;
	ta=sqrt((kt1*kt1)-(4*kt2*(vth25-((float)ptat))))-kt1;
	ta=(ta/(2*kt2))+25;
}

void read_all_pixels(short int* buf)
{
	int handle;
	unsigned char buf_w[4];
	
  handle=I2C_OPEN(I2C_BUS, 0x60);
	buf_w[0]=0x02; // command
  buf_w[1]=0x00; // start address
  buf_w[2]=0x01; // address step
  buf_w[3]=0x40; // number of reads (64)
	I2C_WRITE_READ(handle,0x60, buf_w, 4, 0x60, (unsigned char*)buf, 128);
	I2C_CLOSE(handle);
	
	
}

void read_calc_compens_pixel(float ta, float* compens_pixel_offcomp)
{
	int handle;
	unsigned char buf_w[4];
	unsigned char bscale;
	short int pixbuf;
	short int acp, bcp;
	
  handle=I2C_OPEN(I2C_BUS, 0x60);
	buf_w[0]=0x02; // command
  buf_w[1]=0x91; // start address
  buf_w[2]=0x00; // address step
  buf_w[3]=0x01; // number of reads (64)
	I2C_WRITE_READ(handle,0x60, buf_w, 4, 0x60, (unsigned char*)&pixbuf, 2);
	I2C_CLOSE(handle);
	
		
	acp=EE_BUFFER[0xd4];
	if (acp>127)
		acp=acp-256;
	bcp=EE_BUFFER[0xd5];
	if (bcp>127)
		bcp=bcp-256;
	bscale=EE_BUFFER[0xd9];
	
	*compens_pixel_offcomp=((float)pixbuf) - ( ((float)acp)+( (((float)bcp)/(float)pow(2, (double)bscale)) * (ta-25)));
	
		
}

float pixel_temp(float ta, unsigned char pixel_index, float compens_pixel_offcomp)
{
	float pixeltemp;
	unsigned char pixel_idx;
	short int rawpixel;
	float vcomp;
	unsigned char bscale;
	float voffcomp;
	float vtgccomp;
	short int tgc;
	short int pca;
	short int pcb;
	unsigned short int* em_coeff;
	float em_coeff_val;
	unsigned short int* alpha0;
	unsigned char alphadeltapixel;
	unsigned char alpha0scale;
	unsigned char alphadeltascale;
	float alpha;
	
	pixel_idx=pixel_index;

	// calculate vcomp (7.3.3.1 in datasheet)
	rawpixel=IRTemp_raw[pixel_idx];
	pca=EE_BUFFER[pixel_idx];
	if (pca>127)
		pca=pca-256;
	pcb=EE_BUFFER[0x40+pixel_idx];
	if (pcb>127)
		pcb=pcb-256;
	tgc=EE_BUFFER[0xd8];
	if (tgc>127)
		tgc=tgc-256;
	bscale=EE_BUFFER[0xd9];
	voffcomp=((float)rawpixel)- ( ((float)pca)+( (((float)pcb)/(float)pow(2, (double)bscale)) * (ta-25)));
	vtgccomp=voffcomp-((((float)tgc)/32)*compens_pixel_offcomp);
	
	em_coeff=(unsigned short int*)&EE_BUFFER[0xe4];
	em_coeff_val=((float)(*em_coeff))/32768;
	vcomp=vtgccomp/em_coeff_val;
		
	alphadeltapixel=EE_BUFFER[0x80+pixel_idx];
	alpha0=(unsigned short int*)&EE_BUFFER[0xe0];
	alpha0scale=EE_BUFFER[0xe2];
	alphadeltascale=EE_BUFFER[0xe3];
	alpha=(((float)(*alpha0))/(float)(pow(2, (double)alpha0scale))) + (((float)alphadeltapixel)/((float)pow(2, (double)alphadeltascale)));
	
	pixeltemp=((float)pow((double)((vcomp/alpha)+((float)pow((double)(ta+273.15), 4))), 0.25))-273.15;
	
	return(pixeltemp);
}

void remap_array(short int* buf)
{
	short int tbuf[MLX620_IR_SENSORS];
	short int* buf_ptr;
	buf_ptr=buf;
	int i, j, x;
	x=15;
	for (j=0; j<MLX620_IR_COLUMNS; j++)
	{
		for (i=0; i<MLX620_IR_ROWS; i++)
		{
			tbuf[(i*16)+x]=*buf_ptr++;
		}
		x--;
	}	
	for (i=0; i<MLX620_IR_SENSORS; i++)
	{
		buf[i]=tbuf[i];
	}
}

void dump(short int* buf)
{
	unsigned char* buf_ptr;
	buf_ptr=(unsigned char *)buf;
	unsigned char ms, ls;
	
	int x, y;
	for (y=0; y<MLX620_IR_ROWS; y++)
	{
		for (x=0; x<MLX620_IR_COLUMNS; x++)
		{
			ls=*buf_ptr++;
			ms=*buf_ptr++;
		  printf("%02x%02x ", ms, ls);
		  if (x==7) printf(" ");
		}
		printf("\n");
	}
}

void MEASURE_TEMP(float (*IRTemp)[16])
{
unsigned int ptat;
  float vth25;
  float kt1, kt2;
  float ta;
  float compens_pixel_offcomp;
  float pt;
  

 // decode ta coefficients from EEPROM
  calc_ta_coeff(&vth25, &kt1, &kt2);
 // read temperature register
  ptat=ptat_read();
  ta=calc_ta(ptat, vth25, kt1, kt2);
  printf("Temperature of chip is %.2f degrees C\n", ta);
  read_calc_compens_pixel(ta, &compens_pixel_offcomp);
  read_all_pixels(IRTemp_raw);
  remap_array(IRTemp_raw);
  for (int i=0; i < MLX620_IR_ROWS; i++)
  {
    for( int j=0; j < MLX620_IR_COLUMNS ; j++)
	{
	   	IRTemp[i][j]=pixel_temp(ta, i, compens_pixel_offcomp);
	}
		
  }
  
}

void CUSTOM_PRINT(float (*t)[16]){

for (int i=0; i < MLX620_IR_ROWS; i++)
  {
	printf("\n");
    for( int j=0;j < MLX620_IR_COLUMNS ; j++)
	{
		printf("%f\t",t[i][j]);
                    
	}
		
  }
  printf("\n");
   
}
float minimum(float (*array)[16]){
    float min=array[0][0];
	for (int i=0; i < MLX620_IR_ROWS; i++)
	{
		for( int j=0;j < MLX620_IR_COLUMNS ; j++)
		{
			if(array[i][j]<min)
				min=array[i][j];	
		}
		
	}
  
    return min;
}

float maximum(float (*array)[16]){
    float max=array[0][0];
    for (int i=0; i < MLX620_IR_ROWS; i++)
	{
		for( int j=0;j < MLX620_IR_COLUMNS ; j++)
		{
			if(array[i][j]>max)
				max=array[i][j];	
		}

		
	}
    return max;
}


void NORMALIZE(float (*array)[16]){
    float min=minimum(array);
    float max=maximum(array);
    
    printf("\n \n Min=%f \t Max=%f \n",min,max);
	for (int i=0; i < MLX620_IR_ROWS; i++)
	{
		for( int j=0;j < MLX620_IR_COLUMNS ; j++)
		{
			if(array[i][j]<25)
				array[i][j]=0;
			else if(array[i][j]>35)
				array[i][j]=255;
			
			
			//array[i][j]=((array[i][j]-min)*(255/(max-min)));	
		}
		
	}
    
}

void frame(uint8_t (*pIRTemp)[16])
{
  int i, ACK;
  float IRTemp[MLX620_IR_ROWS][MLX620_IR_COLUMNS];
  
	for (i=0; i < 5; i++)
	{ 
  	ACK=INITIALIZE(); 
	
  	if (ACK<0 && i==4)
  	{
  		printf("Sensor initialization failed. Aborting !!! \n");
  		//return(1);
		exit(1);  		
  	}
  	else if ( ACK<0 && i<4)
	{
		printf("Failed to initialize sensor, attempting again\n");
  		WAIT(50);
  		continue;
	}
	else
	{
  		break;
  	}
    }
	
    MEASURE_TEMP(IRTemp);
	//CUSTOM_PRINT(IRTemp);
	NORMALIZE(IRTemp);
//	printf("\n Normalized values: \n");
//	CUSTOM_PRINT(IRTemp);
//       uint8_t pIRTemp[4][16];

       for(int i=0;i<4;i++){
	for(int j=0;j<16 ; j++)
	{
         pIRTemp[i][j]=uint8_t(IRTemp[i][j]);
 	}
	}

 
 
}

