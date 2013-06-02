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
Serial uart(p28, p27);     // TX, RX
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

char commandPan[15][6] =
{"R0060",
"R0050",
"R0040",
"R0030",
"R0020",
"R0010",
"Z0000",
"Z0000",
"Z0000",
"L0010",
"L0020",
"L0030",
"L0040",
"L0050",
"L0060"};

char commandTilt[3][6] =
{"U0010",
"Z0000",
"D0010"};

#define NUMBER_MOVING_SAMPLE 8 
#define SLIDING_WINDOW_SIZE 2 

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
    
void periodicOutput ()
{
        static int periodicCounter = 0;
        
        periodicCounter++ ;
        
       
        ack = MLX90620_MeasureTemperature(IRtempC, &Ta);

        if(ack == MLX620_ACK)
        {
           
            movingAverage[movingAverageCounter++]->importGrid(IRtempC);
            if (movingAverageCounter >= NUMBER_MOVING_SAMPLE)
            {
                movingAverageCounter = 0;
            }
            
            filterOutput.gridAverage(movingAverage,NUMBER_MOVING_SAMPLE);
            
            extrapolateGrid.interpolateFrom(&filterOutput, INTERPOLATION_SCALE);

            trackingGrid.calculateSumGrid(&extrapolateGrid,SLIDING_WINDOW_SIZE);            
            
            //Brigthen the hotest window
            extrapolateGrid.setValue(trackingGrid.getMaxRowIndex(), trackingGrid.getMaxColumnIndex(),100.0);            
            
            //  if ( periodicCounter % 4 == 0)
            //  {
                pc.printf("\nAmbient T= %2.1f\n", Ta);            
                pc.printf("IR: ");
                for(int column = 0; column < (MLX620_IR_COLUMNS_EXT); column++)
                {
                    for(int row = 0; row < (MLX620_IR_ROWS_EXT) ; row++)
                    { 
               
                        //pc.printf("%2.1f ",IRtempC[pixIdx]);
                        //pc.printf("%2.1f ",inputGrid.getValue(pixIdx / MLX620_IR_COLUMNS, pixIdx % MLX620_IR_COLUMNS ));
                        //pc.printf("%2.1f ",extrapolateGrid.getValue(pixIdx / MLX620_IR_COLUMNS_EXT, pixIdx % MLX620_IR_COLUMNS_EXT));
                        pc.printf("%2.1f ",extrapolateGrid.getValue(row, column));
                    }
                //pc.printf("\n");                        
                }
                uart.printf("STD%c%s%sAA",packetNum,commandPan[trackingGrid.getMaxColumnIndex()], commandTilt[trackingGrid.getMaxRowIndex()]);
            }
        //}
}


int main(void)
{
  uint16_t trimReg, confReg;
  
  MLX620_I2C_Driver_Init (3,3,3,3);

  pc.baud(115200);
  uart.baud(19200);
      
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
  
  period125ms.attach(&periodicOutput, (0.125));
            
  for (;;)
  {        
          
        
  }
}
