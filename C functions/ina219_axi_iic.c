#include "xiic.h"
#include "xparameters.h"
#include <stdio.h>
#include "INA219.h"

#define INA219_ADDRESS  0x40 // Default I2C address
#define INA219_REG_CONFIG  0x00
#define INA219_REG_SHUNTVOLTAGE  0x01
#define INA219_REG_BUSVOLTAGE  0x02
#define INA219_REG_POWER  0x03
#define INA219_REG_CURRENT  0x04
#define INA219_REG_CALIBRATION  0x05

#define IIC_DEVICE_ID XPAR_IIC_0_DEVICE_ID

XIic Iic;

int IicInit()
{
    return XIic_Initialize(&Iic, IIC_DEVICE_ID);
}

int IicWriteReg(u8 Reg, u16 Value)
{
    u8 Buffer[3];
    Buffer[0] = Reg;
    Buffer[1] = (Value >> 8) & 0xFF;
    Buffer[2] = Value & 0xFF;
    return XIic_Send(Iic.BaseAddress, INA219_ADDRESS, Buffer, 3, XIIC_STOP);
}

int IicReadReg(u8 Reg, u16 *Value)
{
    u8 Buffer[2];
    XIic_Send(Iic.BaseAddress, INA219_ADDRESS, &Reg, 1, XIIC_REPEATED_START);
    XIic_Recv(Iic.BaseAddress, INA219_ADDRESS, Buffer, 2, XIIC_STOP);
    *Value = (Buffer[0] << 8) | Buffer[1];
    return 0;
}

int INA219_Init()
{
    IicInit();
    return IicWriteReg(INA219_REG_CALIBRATION, 0x1000);
}

float INA219_ReadShuntVoltage()
{
    u16 Value;
    IicReadReg(INA219_REG_SHUNTVOLTAGE, &Value);
    return (float)((int16_t)Value) * 0.01; // Convert to mV
}

float INA219_ReadBusVoltage()
{
    u16 Value;
    IicReadReg(INA219_REG_BUSVOLTAGE, &Value);
    return (float)(Value >> 3) * 4.0; // Convert to V
}

float INA219_ReadCurrent()
{
    u16 Value;
    IicReadReg(INA219_REG_CURRENT, &Value);
    return (float)((int16_t)Value) * 0.1; // Convert to mA
}

float INA219_ReadPower()
{
    u16 Value;
    IicReadReg(INA219_REG_POWER, &Value);
    return (float)Value * 2.0; // Convert to mW
}
