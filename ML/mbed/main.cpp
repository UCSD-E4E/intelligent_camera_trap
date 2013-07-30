/*
*  Copyright (C) 2011 Melexis N.V.
*  $RCSfile: MLX620_Demo.c,v $
*  $Author: dal $
*  $Date: 2013/04/19 09:52:47 $
*  $Revision: 1.6 $
*/
#include "mbed.h"
#include "grid.h"
#include <stdlib.h>
#include "MLX620_API.h"
#include "MLX620_Sensor.h"
#include "motor.h"
#include "state.h"
#include "tracking.h"

//Serial pc(USBTX, USBRX); // tx, rx
Serial motorControlUart(p28, p27);     // TX, RX
Ticker period125ms;

Serial xbee(USBTX, USBRX); //tx, rx

DigitalOut offSignal(p19);
DigitalOut countdownLED(LED1);

MLX620_SENSOR* mlx620_0;
DigitalInOut pin_sda_0(p13);
DigitalInOut pin_scl_0(p14);

MLX620_SENSOR* mlx620_1;
DigitalInOut pin_sda_1(p15);
DigitalInOut pin_scl_1(p16);

MLX620_SENSOR* mlx620_2;
DigitalInOut pin_sda_2(p17);
DigitalInOut pin_scl_2(p18);

MLX620_SENSOR* mlx620_3;
DigitalInOut pin_sda_3(p22);
DigitalInOut pin_scl_3(p21);

MOTOR* ninjaTurtle;

uint8_t ack_0;      //I2C acknowledge bit
double Ta_0;        //Ambient Temperature
double IRtempC_0[MLX620_IR_SENSORS];

uint8_t ack_1;      //I2C acknowledge bit
double Ta_1;        //Ambient Temperature
double IRtempC_1[MLX620_IR_SENSORS];

uint8_t ack_2;      //I2C acknowledge bit
double Ta_2;        //Ambient Temperature
double IRtempC_2[MLX620_IR_SENSORS];

uint8_t ack_3;      //I2C acknowledge bit
double Ta_3;        //Ambient Temperature
double IRtempC_3[MLX620_IR_SENSORS];

int num_sensors;

void periodicOutput ()
{
    num_sensors = 2;
        //static GRID filterOutput_0(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
        static GRID filterOutput_1(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
        static GRID filterOutput_2(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
      //  static GRID filterOutput_3(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
        static GRID stackedGrid   (MLX620_IR_ROWS*num_sensors,MLX620_IR_COLUMNS);
        static GRID* stackedHelper [2] = { &filterOutput_1, &filterOutput_2}; //, &filterOutput_2 }; //,&filterOutput_2, &filterOutput_3
        
        static GRID extrapolateGrid(MLX620_IR_ROWS_EXT,MLX620_IR_COLUMNS_EXT);
        //static GRID trackingGrid(MLX620_IR_ROWS_EXT - (SLIDING_WINDOW_SIZE - 1),MLX620_IR_COLUMNS_EXT - (SLIDING_WINDOW_SIZE - 1));
        //static int trackRowPos = extrapolateGrid.getRow() / 2;
        //static int trackColumnPos = extrapolateGrid.getColumn() / 2; 
        static int state = STATE_INIT;
        static int counter = 0;
        bool tracking = false;        
        
        //ack_0 = mlx620_0->MLX90620_MeasureTemperature(IRtempC_0, &Ta_0);
        ack_1 = mlx620_1->MLX90620_MeasureTemperature(IRtempC_1, &Ta_1);
        ack_2 = mlx620_2->MLX90620_MeasureTemperature(IRtempC_2, &Ta_2);
       //ack_3 = mlx620_3->MLX90620_MeasureTemperature(IRtempC_3, &Ta_3);
       ack_3 = MLX620_ACK;
       ack_0 = MLX620_ACK;
       
        if(ack_0 == MLX620_ACK && ack_1 == MLX620_ACK && ack_2 == MLX620_ACK && ack_3 == MLX620_ACK)
        {
           
            //filterOutput_0.importGrid(IRtempC_0);
            filterOutput_1.importGrid(IRtempC_1);
            filterOutput_2.importGrid(IRtempC_2);
           // filterOutput_3.importGrid(IRtempC_3);
            
            stackedGrid.importGridArray(stackedHelper,2);
     
            //extrapolateGrid.interpolateFrom(&filterOutput, INTERPOLATION_SCALE);
            /*
            //tracking decision ?
            if (extrapolateGrid.getMax() > extrapolateGrid.getAvg() * ADAPTIVE_THRESHOLD)
            {            
                tracking = true;
            }else
            {
                tracking = false;                
            }  

            if (tracking == true) {
            
                trackingGrid.calculateSumGrid(&extrapolateGrid,SLIDING_WINDOW_SIZE);           
            
                if ( trackingGrid.getMaxRowIndex() > trackRowPos)
                {
                    trackRowPos++;
                
                } else if ( trackingGrid.getMaxRowIndex() < trackRowPos)
                {
                    trackRowPos--;
                }
            
                if ( trackingGrid.getMaxColumnIndex() > trackColumnPos)
                {
                    trackColumnPos++;
                
                } else if ( trackingGrid.getMaxColumnIndex() < trackColumnPos)
                {
                    trackColumnPos--;
                }
            
                //Brigthen the hotest window
                extrapolateGrid.setValue(trackRowPos, trackColumnPos,100.0);            
            }
            */ 
                xbee.printf("\nAmbient T=%2.1f\n", Ta_0);   
                //pc.printf("\nSensor 1 Ambient T= %2.1f\n", Ta_1);         
                xbee.printf("IR: ");
                int countnumtemps = 0;
                for(int column = 0; column < stackedGrid.getColumn(); column++)
                {
                   // pc.printf("\n");
                    for(int row = 0; row < stackedGrid.getRow() ; row++)
                    { 
                    
                        
                        xbee.printf(" %2.1f",stackedGrid.getValue(row, column));
                    }
                }
                //pc.printf("\n");
                
                
                //if(tracking == true && ((state == STATE_TRACKING) || (state == STATE_TURN_CAMERA_ON)))
                //{
                //    ninjaTurtle->sendCommand(trackRowPos,3,trackColumnPos,15);                    
                //}
            }
                
        counter++;        
        
/*        if (state == STATE_INIT)
        {
            //Riley: TURN Camera ON
            
            state = STATE_TURN_CAMERA_ON;
            counter = 0;
        }else if (state == STATE_TURN_CAMERA_ON)
        {
            if (counter > TIME_FOR_STATE_TURN_CAMERA_ON_S)
            {
                //Riley: Finish TURN Camera ON
                state = STATE_TRACKING;
                counter = 0;
            }
        
        }else if (state == STATE_TRACKING)
        {
            if (tracking == true)
            {
                counter = 0;
            }
            else if (counter  > TIME_OUT_FOR_NOT_TRACKING)
            {
                counter = 0 ;
                state = STATE_TURN_CAMERA_OFF;
                //Pan / Tilt Center
                ninjaTurtle->center();
 
                //Riley: TURN Camera OFF           
            
            }
        
        }else if (state == STATE_TURN_CAMERA_OFF)        
        {
            if (counter  > TIME_FOR_STATE_TURN_CAMERA_OFF_S)
            {
                counter = 0 ;
                state = STATE_SHUTTING_DOWN;
                //Riley: Finish TURN Camera OFF               
            
            }
        
        } 
        else
        {
            wait(3.0); // Wait for position to center            
            //Sam : Signal Hipervisor Ready to shutdown            
        
        }  */      
        
}

void kill_power(){
    offSignal = 1;
    wait(.01);
    offSignal = 0;
    }

int main(void)
{
    offSignal = 0;
    countdownLED = 0;

        
  
  uint16_t trimReg, confReg;
  //ninjaTurtle = new MOTOR(&motorControlUart);
  
  xbee.baud(115200); 
  //pc.printf("Welcome!\n");
  
  /*mlx620_0 = new MLX620_SENSOR(&pin_sda_0, &pin_scl_0); 
  mlx620_0->i2c_port->MLX620_I2C_Driver_Init (3,3,3,3);
  ack_0 = mlx620_0->MLX90620_InitializeSensor(&trimReg, &confReg);
    
  if (ack_0 == MLX620_ACK)
  {
    //pc.printf("Sensor 0 initialized successfully\n");
    //pc.printf("Triming Register = %X\n, trimReg");
    //pc.printf("Configuration Register = %X\n, confReg");
  }
  else
  {
    xbee.printf("ERROR: Sensor 0 initiazation failed!\n");
  }
  */
  mlx620_1 = new MLX620_SENSOR(&pin_sda_1, &pin_scl_1); 
  mlx620_1->i2c_port->MLX620_I2C_Driver_Init (3,3,3,3);
  ack_1 = mlx620_1->MLX90620_InitializeSensor(&trimReg, &confReg);
    
  if (ack_1 == MLX620_ACK)
  {
 //   pc.printf("Sensor 1 initialized successfully\n");
    //pc.printf("Triming Register = %X\n, trimReg");
    //pc.printf("Configuration Register = %X\n, confReg");
  }
  else
  {
    xbee.printf("ERROR: Sensor 1 initiazation failed!\n");
  }
  mlx620_2 = new MLX620_SENSOR(&pin_sda_2, &pin_scl_2); 
  mlx620_2->i2c_port->MLX620_I2C_Driver_Init (3,3,3,3);
  ack_2 = mlx620_2->MLX90620_InitializeSensor(&trimReg, &confReg);
    
   if (ack_2 == MLX620_ACK)
  {
   xbee.printf("Sensor 2 initialized successfully\n");
    //pc.printf("Triming Register = %X\n, trimReg");
    //pc.printf("Configuration Register = %X\n, confReg");
  }
  else
  {
    xbee.printf("ERROR: Sensor  2 initiazation failed!\n");
  }
  
  /*mlx620_3 = new MLX620_SENSOR(&pin_sda_3, &pin_scl_3); 
  mlx620_3->i2c_port->MLX620_I2C_Driver_Init (3,3,3,3);
  ack_3 = mlx620_3->MLX90620_InitializeSensor(&trimReg, &confReg);
    
  if (ack_3 == MLX620_ACK)
  {
    pc.printf("Sensor initialized successfully\n");
    //pc.printf("Triming Register = %X\n, trimReg");
    //pc.printf("Configuration Register = %X\n, confReg");
  }
  else
  {
    pc.printf("ERROR: Sensor initiazation failed!\n");
  }*/
  
  //wait(3.0);
  //period125ms.attach(&periodicOutput, (0.125*2));
  
  period125ms.attach(&periodicOutput, (0.125)); 
   //while(1){xbee.printf("canyouhearmenow?");}
  int c = 10;
  /*while(c){
    countdownLED = !countdownLED;
    wait(.2);
    c--;
   }
  kill_power(); //FIGURE OUT WHY THIS ISN"T WORKING!!
  */for (;;)
  {        
      
        
  }
}
