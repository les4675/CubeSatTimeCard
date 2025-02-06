#ifndef INA219_AXI_IIC_H
#define INA219_AXI_IIC_H

#include "xiic.h"
#include "xparameters.h"
#include <stdio.h>

#define INA219_ADDRESS  0x40 // Default I2C address
#define INA219_REG_CONFIG  0x00
#define INA219_REG_SHUNTVOLTAGE  0x01
#define INA219_REG_BUSVOLTAGE  0x02
#define INA219_REG_POWER  0x03
#define INA219_REG_CURRENT  0x04
#define INA219_REG_CALIBRATION  0x05

#define IIC_DEVICE_ID XPAR_IIC_0_DEVICE_ID

int IicInit();
int IicWriteReg(u8 Reg, u16 Value);
int IicReadReg(u8 Reg, u16 *Value);
int INA219_Init();
float INA219_ReadShuntVoltage();
float INA219_ReadBusVoltage();
float INA219_ReadCurrent();
float INA219_ReadPower();

#endif // INA219_AXI_IIC_H
