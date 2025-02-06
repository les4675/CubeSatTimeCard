# INA219 AXI IIC Implementation Guide

## Overview
This document provides guidance on implementing the INA219 current/power monitor with a Xilinx Zynq-7000 SoC using the AXI IIC interface.

## Prerequisites
- Xilinx Vivado and Vitis installed
- Xilinx Zynq-7000 SoC-based board
- INA219 sensor module
- AXI IIC enabled in the hardware design

## Hardware Setup
1. Connect the INA219 sensor module to the Zynq board using the I2C lines:
   - **SDA** -> Connect to AXI IIC SDA pin
   - **SCL** -> Connect to AXI IIC SCL pin
   - **GND** -> Ground
   - **VCC** -> 3.3V or 5V (based on INA219 requirements)

2. Enable the AXI IIC in the Vivado block design:
   - Add an **AXI IIC** IP block
   - Connect it to the **Processor System Reset**
   - Map its **SDA** and **SCL** pins to the appropriate board pins
   - Validate and generate the bitstream

## Software Implementation
### 1. Include the INA219 AXI IIC Library
```c
#include "ina219_axi_iic.h"
```

### 2. Initialize the I2C Interface and INA219 Sensor
```c
int main() {
    if (IicInit() != XST_SUCCESS) {
        printf("I2C Initialization Failed!\n");
        return -1;
    }
    
    if (INA219_Init() != XST_SUCCESS) {
        printf("INA219 Initialization Failed!\n");
        return -1;
    }
    
    printf("INA219 Initialized Successfully!\n");
    return 0;
}
```

### 3. Read Sensor Data
```c
float voltage = INA219_ReadBusVoltage();
float current = INA219_ReadCurrent();
float power = INA219_ReadPower();

printf("Bus Voltage: %.2f V\n", voltage);
printf("Current: %.2f mA\n", current);
printf("Power: %.2f mW\n", power);
```

## Debugging
- Ensure correct I2C addresses are being used.
- Use an oscilloscope or logic analyzer to verify I2C signals.
- Check if the AXI IIC IP is properly configured in Vivado.

## Conclusion
This guide provides the steps to interface the INA219 with a Xilinx Zynq-7000 SoC using the AXI IIC. By following the steps, you should be able to measure bus voltage, current, and power successfully.

