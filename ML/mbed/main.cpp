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

Serial pc(USBTX, USBRX); // tx, rx
Serial motorControlUart(p28, p27);     // TX, RX
Ticker period125ms;

double IRtempC[MLX620_IR_SENSORS];

/**
 * \file MLX620_Demo.C
 * \brief MLX90620 API functions demonstration usage.
 * \details This demonstration usage of the API functions performing the most necessary operations in order to get full frame of the sensor printed out using standard 'printf' function.\n
 * The users of this demo should implement the 'printf' function for the particular MCU and compiler that is used. If the printing functionality is not needed, it should be disabled.
 * This demo is performing the following operations:
 * - initializing the sensor and reporting (printing) the error
 * - printing the values of the "Configuration" and "Trimming" Registers, after the initialization is done
 * - reading raw IR temperature data for each pixel, as well as reading Ambient Temperature sensor
 * - compensating the printing the Ambient Temperature [Kelvin]
 * - compensating the printing the Object's Temperature for each pixel from 1 to 64
 */

/** \fn uint8_t MLX90620_InitializeSensor(uint16_t *trim, uint16_t *conf)
* \brief Initialize the sensor.
* \details It should be used only when the sensor is supplied.
* \param[out] *trim Trimming register value after the configuration is done.
* \param[out] *conf Configuration register value after the configuration is done.
* \retval ack I2C acknowledge bit.
*/
uint8_t MLX90620_InitializeSensor(uint16_t *trim, uint16_t *conf);

/** \fn uint8_t MLX90620_MeasureTemperature(double *pIRtempC, double *Ta)
* \brief Read measurement data from the sensor and calculate ambient and Infra Red (object's) temperature in Kelvin.
* \details The temperature results for each pixel is saved in .\n
* \param[in] *pIRtempC Pointer to buffer where the temperature results for each pixel would be stored
* \param[out] *Ta Ambient temperature in Kelvin
* \retval ack I2C acknowledge bit.
*/
uint8_t MLX90620_MeasureTemperature(double *pIRtempC, double *Ta);


uint8_t MLX90620_InitializeSensor(uint16_t *trim, uint16_t *conf)
{
  uint8_t ack;

  ack = MLX620_Initialize();      //initialize the sensor
  if (ack == MLX620_ACK)
  {
    ack = MLX620_ReadTrim(trim);    //read the Trimming register and return it
    ack |= MLX620_ReadConfig(conf); //read the Configuration register and return it
  }

  return ack;
}

uint8_t MLX90620_MeasureTemperature(double *pIRtempC, double *Ta)
{
  uint8_t ack;
  int16_t ptat, tgc;

  //get RAW (not compensated) ambient temperature sample (PTAT sensor)
  ack = MLX620_ReadRAM(MLX620_RAM_PTAT, 0, 1, (uint8_t*)&ptat);

  if (ack == MLX620_ACK)
  {
    //compensate ambient temperature; get absolute temperature in Kelvin
    *Ta = MLX620_GetTaKelvin (ptat);

    ack = MLX620_ReadRAM(MLX620_RAM_TGC, 0, 1, (uint8_t*)&tgc);

    if (ack == MLX620_ACK)
    {
      MLX620_CalcTGC(tgc);

      ack = MLX620_ReadRAM(MLX620_RAM_IR_BEG, 1, MLX620_IR_SENSORS, (uint8_t*)MLX620_RAMbuff);
      
      if (ack == MLX620_ACK)
      {
        MLX620_CompensateIR((int16_t*)MLX620_RAMbuff, MLX620_RAM_IR_BEG, 1, MLX620_IR_SENSORS, pIRtempC);
        for (int i = 0; i < MLX620_IR_SENSORS ;i++)
        {
            //Compensate to Celsius
            pIRtempC[i] -= 273.15;
        }
        
        *Ta -= 273.15;
      }
    }
  }

  return ack;
}
/** \fn int main(void)
* \brief main
*/
char packetNum = 11;
<<<<<<< HEAD
char commandCharNum = 1;
=======
>>>>>>> 4a3138645f2b695e7e9c256083f7bbe605d8a38c

char commandPan[15][6] =
{"R0120",
 "R0100",
 "R0060",
 "R0040",
 "R0020",
 "R0010",
 "R0005",
 "R0000",
 "L0005",
 "L0010",
 "L0020",
 "L0040",
 "L0060",
 "L0100",
 "L0120"};

char commandTilt[3][6] =
{"U0020",
 "Z0000",
 "D0020"};

char commandCenter[11] = { "X0500Y0200" };

#define NUMBER_MOVING_SAMPLE 8 
#define SLIDING_WINDOW_SIZE 2 
#define ADAPTIVE_THRESHOLD  1.055

uint8_t ack;      //I2C acknowledge bit
double Ta;        //Ambient Temperature

GRID inputGrid0(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
GRID inputGrid1(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
GRID inputGrid2(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
GRID inputGrid3(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
GRID inputGrid4(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
GRID inputGrid5(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
GRID inputGrid6(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
GRID inputGrid7(MLX620_IR_ROWS,MLX620_IR_COLUMNS);

int movingAverageCounter = 0;
GRID* movingAverage[NUMBER_MOVING_SAMPLE] = {&inputGrid0, &inputGrid1, &inputGrid2, &inputGrid3, &inputGrid4, &inputGrid5, &inputGrid6, &inputGrid7} ;

GRID filterOutput(MLX620_IR_ROWS,MLX620_IR_COLUMNS);

GRID extrapolateGrid(MLX620_IR_ROWS_EXT,MLX620_IR_COLUMNS_EXT);
GRID trackingGrid(MLX620_IR_ROWS_EXT - (SLIDING_WINDOW_SIZE - 1),MLX620_IR_COLUMNS_EXT - (SLIDING_WINDOW_SIZE - 1));

#define STATE_INIT                 0
#define STATE_TURN_CAMERA_ON       1
#define STATE_TRACKING             2
#define STATE_TURN_CAMERA_OFF      3
#define STATE_SHUTTING_DOWN        4

#define TIME_FOR_STATE_TURN_CAMERA_ON_S      (3 * 8)
#define TIME_OUT_FOR_NOT_TRACKING            (5 * 8)
#define TIME_FOR_STATE_TURN_CAMERA_OFF_S     (3 * 8)
    
void periodicOutput ()
{
        static int trackRowPos = extrapolateGrid.getRow() / 2;
        static int trackColumnPos = extrapolateGrid.getColumn() / 2; 
        static  int state = STATE_INIT;
        static int counter = 0;
        
        bool tracking = false;        
        
        ack = MLX90620_MeasureTemperature(IRtempC, &Ta);

        if(ack == MLX620_ACK)
        {
           
            movingAverage[movingAverageCounter++]->importGrid(IRtempC);
            if (movingAverageCounter >= NUMBER_MOVING_SAMPLE)
            {
                movingAverageCounter = 0;
            }
            
            filterOutput.importGridArray(movingAverage,NUMBER_MOVING_SAMPLE);
            
            extrapolateGrid.interpolateFrom(&filterOutput, INTERPOLATION_SCALE);

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
            } else
            {
                motorControlUart.printf("STC%ccAASTC%cdAA",commandCharNum,commandCharNum);
            }
                pc.printf("\nAmbient T= %2.1f\n", Ta);            
                pc.printf("IR: ");
                for(int column = 0; column < (MLX620_IR_COLUMNS_EXT); column++)
                {
                    for(int row = 0; row < (MLX620_IR_ROWS_EXT) ; row++)
                    { 
                        pc.printf("%2.1f ",extrapolateGrid.getValue(row, column));
                    }
                }
<<<<<<< HEAD
                
                if(tracking == true && ((state == STATE_TRACKING) || (state == STATE_TURN_CAMERA_ON)))
                {
                    motorControlUart.printf("STD%c%s%sAA",packetNum,commandPan[trackColumnPos], commandTilt[trackRowPos]);
                }
=======
                uart.printf("STD%c%s%sAA",packetNum,commandPan[trackingGrid.getMaxColumnIndex()], commandTilt[trackingGrid.getMaxRowIndex()]);
>>>>>>> 4a3138645f2b695e7e9c256083f7bbe605d8a38c
            }
                
        counter++;        
        
        if (state == STATE_INIT)
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
                motorControlUart.printf("STC%ccAASTC%cdAA",commandCharNum,commandCharNum);
 
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
            
        
        }
        
        
}


int main(void)
{
  uint16_t trimReg, confReg;
  
  MLX620_I2C_Driver_Init (3,3,3,3);

  pc.baud(115200);
  motorControlUart.baud(19200);
      
  ack = MLX90620_InitializeSensor(&trimReg, &confReg);

  if (ack == MLX620_ACK)
  {
    pc.printf("Sensor initialized successfully\n");
    pc.printf("Triming Register = %X\n, trimReg");
    pc.printf("Configuration Register = %X\n, confReg");
  }
  else
  {
    pc.printf("ERROR: Sensor initiazation failed!\n");
  }
  
  wait(3.0);
  period125ms.attach(&periodicOutput, (0.125));
            
  for (;;)
  {        
          
        
  }
}
