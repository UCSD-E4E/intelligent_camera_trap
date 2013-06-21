/*
*  Copyright (C) 2011 Melexis N.V.
*  $RCSfile: MLX620_I2C_Driver.c,v $
*  $Author: dal $
*  $Date: 2013/04/19 09:52:47 $
*  $Revision: 1.8 $
************************************************************* */

/** \file MLX620_I2C_Driver.c
 *  \brief I2C communication driver source code
 *  \details Do not modify this file if not needed.
 */
#ifndef _MLX620_I2C_H_
#define _MLX620_I2C_H_

#include "mbed.h"
#include "MLX620_I2C_Driver.h"

class MLX620_I2C {

public:

  uint32_t MLX620_I2C_clock;
  uint32_t MLX620_I2C_start;
  uint32_t MLX620_I2C_stop;
  uint32_t MLX620_I2C_W_R;
  
  MLX620_I2C(DigitalInOut* tPin_sda,DigitalInOut* tPin_scl);

  void MLX620_I2C_Driver_SendStart (void);
  void MLX620_I2C_Driver_SendStop (void);
  void MLX620_I2C_Driver_Init (uint32_t clock, uint32_t start, uint32_t stop,uint32_t w_r);
  uint32_t MLX620_I2C_Driver_WriteByte(uint8_t byte);
  uint8_t MLX620_I2C_Driver_ReadByte(uint32_t ack);
  uint32_t MLX620_I2C_Driver_Write(uint8_t slaveAddr,uint32_t nBytes,uint8_t *pData);
  uint32_t MLX620_I2C_Driver_Read(uint8_t slaveAddr,uint32_t nBytes,uint8_t *pData);
  uint32_t MLX620_I2C_Driver_WriteRead(uint8_t slaveAddr, uint32_t nBytesWrite, uint8_t *pWriteData, uint32_t nBytesRead,uint8_t *pReadData);

private: 

  DigitalInOut* pin_sda;
  DigitalInOut* pin_scl;
  
  void MLX620_I2C_Driver_NOPdelay (uint32_t nops);
 
};

#endif 
