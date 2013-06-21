#ifndef _MLX620_SENSOR_H_
#define _MLX620_SENSOR_H_

#include <string.h>
#include <math.h>

#include "mbed.h"
#include "MLX620_API.h"
#include "MLX620_I2C.h"

class MLX620_SENSOR {
public:
  double IRtempC[MLX620_IR_SENSORS];

  MLX620_SENSOR(DigitalInOut* tPin_sda,DigitalInOut* tPin_scl);
  uint8_t MLX90620_InitializeSensor(uint16_t *trim, uint16_t *conf);
  uint8_t MLX90620_MeasureTemperature(double *pIRtempC, double *Ta);
  MLX620_I2C* i2c_port;
  
private:
  uint32_t MLX620_I2C_clock;
  uint32_t MLX620_I2C_start;
  uint32_t MLX620_I2C_stop;
  uint32_t MLX620_I2C_W_R;
  
  uint8_t MLX620_ReadRAM(uint8_t startAddr, uint8_t addrStep, uint8_t nWords,uint8_t *pData);
  uint8_t MLX620_ReadEEPROM(uint8_t startAddr, uint16_t nBytes,uint8_t *pData);
  uint8_t MLX620_WriteConfig(uint16_t configReg);
  uint8_t MLX620_ReadConfig (uint16_t *pConfigReg);
  uint8_t MLX620_WriteTrim(uint16_t trimReg);
  uint8_t MLX620_ReadTrim (uint16_t *pTrimReg);
  uint8_t MLX620_StartSingleMeasurement(void);
  uint32_t MLX620_Initialize(void);
  void MLX620_CompensateIR(int16_t* pFrame, int start, int step, int count, double *pIR);
  double MLX620_GetTaKelvin (int16_t ptat);
  void MLX620_CalcTGC(int16_t tgc);
  uint32_t MLX620_CalcTa(int16_t ptat);
  double MLX620_CalcToKelvin(int idxIr, int16_t data);
  void MLX620_CalcCommonParams(void);
  int16_t MLX620_GetRawIR(uint8_t row, uint8_t column);
  uint16_t MLX620_GetPTAT(void);
  int16_t MLX620_GetTGC(void);

  uint16_t MLX620_RAMbuff[MLX620_RAM_SIZE_WORDS];
  uint8_t MLX620_EEbuff[MLX620_EE_SIZE_BYTES];
  int8_t MLX620_Ai[MLX620_IR_SENSORS];
  double MLX620_Bi[MLX620_IR_SENSORS];
  double MLX620_Alphai[MLX620_IR_SENSORS];
  double MLX620_DVtho;
  double MLX620_DKT1;
  double MLX620_DKT2;
  double MLX620_DLastTa;
  double MLX620_TGC;
  double MLX620_CyclopsAlpha;
  double MLX620_CyclopsA;
  double MLX620_CyclopsB;
  double MLX620_Ke;
  double MLX620_CyclopsData;

};



#endif
