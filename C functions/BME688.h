#ifndef BME688_XILINX_I2C_H
#define BME688_XILINX_I2C_H

#include <stdio.h>
#include "xiicps.h"  // Xilinx PS I2C library
#include "xparameters.h"  // Hardware parameters

#define BME688_I2C_ADDR  0x76  // Default I2C address

// I2C instance
extern XIicPs IicInstance;

// Function prototypes
int BME688_Init();
int BME688_ReadData(uint8_t reg, uint8_t *data, int length);
int BME688_WriteData(uint8_t reg, uint8_t data);
float BME688_GetTemperature();
float BME688_GetHumidity();
float BME688_GetPressure();
float BME688_GetGasResistance();

#endif // BME688_XILINX_I2C_H
