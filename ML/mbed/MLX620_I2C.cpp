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
#include "mbed.h"
#include "MLX620_I2C.h"

MLX620_I2C::MLX620_I2C(DigitalInOut* tPin_sda,DigitalInOut* tPin_scl)
{
  pin_sda = tPin_sda;
  pin_scl = tPin_scl;  
}

void MLX620_I2C::MLX620_I2C_Driver_NOPdelay (uint32_t nops)
{
  uint32_t i;

  for(i = 0; i < (nops >> 2) ; i++)
  {
    _NOP();
  }

}

void MLX620_I2C::MLX620_I2C_Driver_SendStart (void)
{
  uint32_t delay = MLX620_I2C_start;

  MLX620_I2C_Driver_NOPdelay(delay);
  pin_scl->input();
  MLX620_I2C_Driver_NOPdelay(delay);

  pin_sda->output();
  MLX620_I2C_Driver_NOPdelay(delay);

}

void MLX620_I2C::MLX620_I2C_Driver_SendStop (void)
{
  uint32_t delay = MLX620_I2C_stop;

  pin_sda->output();
  MLX620_I2C_Driver_NOPdelay(delay);

  pin_scl->input();
  MLX620_I2C_Driver_NOPdelay(delay);

  pin_sda->input();
  MLX620_I2C_Driver_NOPdelay(delay);
}

void MLX620_I2C::MLX620_I2C_Driver_Init (uint32_t clock,
                            uint32_t start,
                            uint32_t stop,
                            uint32_t w_r)
{
  pin_sda->mode(PullUp);
  pin_scl->mode(PullUp);
  *pin_sda = 0;
  *pin_scl = 0;
  
  MLX620_I2C_clock = clock;
  MLX620_I2C_start = start;
  MLX620_I2C_stop = stop;
  MLX620_I2C_W_R = w_r;
}

uint32_t MLX620_I2C::MLX620_I2C_Driver_WriteByte(uint8_t byte)
{
  uint32_t i, clock;

  clock = MLX620_I2C_clock;

  for (i = 0; i < 8; i ++)
  {
    pin_scl->output();
    if (byte & (1<<7))
    {
      pin_sda->input();
    }
    else
    {
      pin_sda->output();
    }
    MLX620_I2C_Driver_NOPdelay(clock);
    pin_scl->input();
    MLX620_I2C_Driver_NOPdelay(clock);
    byte <<= 1;
  }

  _NOP();
  _NOP();

  pin_scl->output();
  pin_sda->input();
  _NOP();
  MLX620_I2C_Driver_NOPdelay(clock);
  _NOP();
  _NOP();
  _NOP();
  pin_scl->input();
  
  if (pin_sda->read())
    i = MLX620_NACK;  //reuse i as acknowledge status
  else
    i = MLX620_ACK;
   
 MLX620_I2C_Driver_NOPdelay(clock);
  _NOP();

  pin_scl->output();

  return i;
}

uint8_t MLX620_I2C::MLX620_I2C_Driver_ReadByte(uint32_t ack)
{
  uint32_t i, clock, data;

   clock = MLX620_I2C_clock;
   data = 0;

   pin_scl->output();
   pin_sda->input();

   for (i = 0; i < 8; i ++)
   {
     pin_scl->output();
     _NOP();
     _NOP();
     _NOP();
     data <<= 1;

     MLX620_I2C_Driver_NOPdelay(clock);

     pin_scl->input();  

     if (pin_sda->read())
       data |= 1;
     else
       data &= ~1;

     MLX620_I2C_Driver_NOPdelay(clock);
   }   
   
   _NOP();
   pin_scl->output();
   if (ack)
     pin_sda->output();
   else
     pin_sda->input();

   _NOP();
   MLX620_I2C_Driver_NOPdelay(clock);

   pin_scl->input();
   _NOP();
   _NOP();
   _NOP();
   MLX620_I2C_Driver_NOPdelay(clock);
   _NOP();
   _NOP();

   pin_scl->output();
   pin_sda->input();

   return ((uint8_t)data);
}

uint32_t MLX620_I2C::MLX620_I2C_Driver_Write(uint8_t slaveAddr,
                                  uint32_t nBytes,
                                  uint8_t *pData)
{
    uint32_t i, ack;

    slaveAddr <<= 1;
    slaveAddr &= ~1;    //W/R bit = W
    ack = 0;
    
    MLX620_I2C_Driver_SendStart();

    ack = MLX620_I2C_Driver_WriteByte(slaveAddr);

    for(i = 0; i < nBytes; i++)
    {
        ack |= MLX620_I2C_Driver_WriteByte(*pData++);
    }

    MLX620_I2C_Driver_SendStop();

    return ack;
}
uint32_t MLX620_I2C::MLX620_I2C_Driver_Read(uint8_t slaveAddr,
                                 uint32_t nBytes,
                                 uint8_t *pData)
{
    uint32_t i, ack;

    slaveAddr <<= 1;
    slaveAddr |= 1;    //W/R bit = R
    
    MLX620_I2C_Driver_SendStart();

    ack = MLX620_I2C_Driver_WriteByte(slaveAddr);


    for(i = 0; i < nBytes - 1; i++)
    {   
        *pData++ = MLX620_I2C_Driver_ReadByte(1);
    }
    *pData = MLX620_I2C_Driver_ReadByte(0);

    MLX620_I2C_Driver_SendStop();

    return ack;
}
uint32_t MLX620_I2C::MLX620_I2C_Driver_WriteRead(uint8_t slaveAddr,
                                    uint32_t nBytesWrite,
                                    uint8_t *pWriteData,
                                    uint32_t nBytesRead,
                                    uint8_t *pReadData)
{
    uint32_t i, ack, delay;

    slaveAddr <<= 1;
    slaveAddr &= ~1;  //W/R bit = W
    ack = 0;
    delay = MLX620_I2C_W_R;

    if (nBytesWrite)
    {
        MLX620_I2C_Driver_SendStart();

        ack = MLX620_I2C_Driver_WriteByte(slaveAddr);

        for(i = 0; i < nBytesWrite; i++)
        {
            ack |= MLX620_I2C_Driver_WriteByte(*pWriteData++);
        }
    }

    if (nBytesRead)
    {
        MLX620_I2C_Driver_NOPdelay(delay);

        slaveAddr |= 1;  //W/R bit = R
        MLX620_I2C_Driver_SendStart();

        ack |= MLX620_I2C_Driver_WriteByte(slaveAddr);

        for(i = 0; i < nBytesRead - 1; i++)
        {
            *pReadData++ = MLX620_I2C_Driver_ReadByte(1);
        }
    *pReadData = MLX620_I2C_Driver_ReadByte(0);
    }

    if (nBytesWrite || nBytesRead)
        MLX620_I2C_Driver_SendStop();

    return ack;

}

