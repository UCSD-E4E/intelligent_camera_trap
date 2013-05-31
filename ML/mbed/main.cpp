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
<<<<<<< HEAD
Serial uart(p28, p27);     // TX, RX
=======
Serial uart(p9, p10); // tx, rx
>>>>>>> origin/master

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
char packetNum = 10;

char commandPan[15][6] =
<<<<<<< HEAD
{"L0060",
"L0050",
"L0040",
"L0030",
"L0020",
"L0010",
"Z0000",
"Z0000",
"Z0000",
"R0010",
"R0020",
"R0030",
"R0040",
"R0050",
"R0120"};

char commandTilt[3][6] =
{"U0010",
"Z0000",
"D0010"};
=======
{"L0120",
"L0100",
"L0080",
"L0060",
"L0040",
"L0020",
"Z0000",
"Z0000",
"Z0000",
"R0020",
"R0040",
"R0060",
"R0080",
"R0100",
"R0120"};

char commandTilt[3][6] =
{"U0020",
"Z0000",
"D0020"};
>>>>>>> origin/master


int main(void)
{
  uint8_t ack;      //I2C acknowledge bit
  double Ta;        //Ambient Temperature
  uint32_t pixIdx;   //pixel index
  uint16_t trimReg, confReg;
  
  GRID inputGrid(MLX620_IR_ROWS,MLX620_IR_COLUMNS);
<<<<<<< HEAD
  GRID extrapolateGrid(MLX620_IR_ROWS_EXT,MLX620_IR_COLUMNS_EXT);
  GRID trackingGrid(MLX620_IR_ROWS_EXT - 1,MLX620_IR_COLUMNS_EXT - 1);
  
  double testCase_topHot [64] = 
  { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
  
  double testCase_bottomHot [64] = 
  { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 };
   
  
  double testCase_leftHot [64] = 
  { 40, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    40, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    40, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    40, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };
       
  double testCase_rightHot [64] = 
  { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 40,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 40,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 40,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 40 };
      
  
  MLX620_I2C_Driver_Init (3,3,3,3);
=======
  GRID trackingGrid(MLX620_IR_ROWS - 1,MLX620_IR_COLUMNS - 1);
  
  MLX620_I2C_Driver_Init (1,1,1,1);
>>>>>>> origin/master

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

            
    for (;;)
    {
        
      if(ack == MLX620_ACK)
      {
        ack = MLX90620_MeasureTemperature(IRtempC, &Ta);

        if(ack == MLX620_ACK)
        {
<<<<<<< HEAD
            pc.printf("\nAmbient T= %2.1f\n", Ta);
            inputGrid.importGrid(IRtempC);
            extrapolateGrid.interpolateFrom(&inputGrid, INTERPOLATION_SCALE);

            //trackingGrid.calculateSumGrid(&extrapolateGrid,3);            
            //trackingGrid.updateMaxInformation();        
            
            pc.printf("IR: ");
            for(int column = 0; column < (MLX620_IR_COLUMNS_EXT); column++)
=======
            pc.printf("Ambient T= %4.1f\n", Ta);
            inputGrid.cloneGrid(IRtempK);
            trackingGrid.calculateSumGrid(&inputGrid,2);            
            trackingGrid.updateMaxInformation();        
            
            pc.printf("IR:");
            for(pixIdx = 0; pixIdx < MLX620_IR_SENSORS; pixIdx++)
>>>>>>> origin/master
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
<<<<<<< HEAD
            //uart.printf("STD%c%s%sA",packetNum,commandPan[trackingGrid.getMaxColumnIndex()], commandTilt[trackingGrid.getMaxRowIndex()]);
            wait_ms(50);
=======
            pc.printf("\n");            
            uart.printf("STC%c%s%sA",packetNum,commandPan[trackingGrid.getMaxColumnIndex()], commandTilt[trackingGrid.getMaxRowIndex()]);
        
>>>>>>> origin/master
        }
        else
        {
            pc.printf("ERROR: Reading data from the sensor failed!\n");
        }
      }
      else
      {
        return 0;
      }
      
        
      }
}
