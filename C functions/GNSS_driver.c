#include <stdio.h>
#include "xparameters.h"
#include "xiic.h"
#include "xil_printf.h"
#include "xil_io.h"

#define IIC_DEVICE_ID      XPAR_IIC_0_DEVICE_ID
#define GNSS_I2C_ADDRESS   0x42  // MAX-M10S I2C address
#define IIC_SCLK_RATE      400000  // Adjusted for Fast-mode I2C (400 kbit/s)

#define I2C_REG_DATA_READY  0xFD  // Register holding available bytes
#define I2C_REG_DATA        0xFF  // Register holding data bytes

#define AXI_NUMA_BASEADDR   XPAR_NUMA_PARSER_0_S00_AXI_BASEADDR
#define AXI_NUMA_STATUS     (AXI_NUMA_BASEADDR + 0x00)
#define AXI_NUMA_LAT        (AXI_NUMA_BASEADDR + 0x04)
#define AXI_NUMA_LON        (AXI_NUMA_BASEADDR + 0x08)
#define AXI_NUMA_TIME       (AXI_NUMA_BASEADDR + 0x0C)
#define AXI_NUMA_CMD        (AXI_NUMA_BASEADDR + 0x10)

XIic IicInstance;

int I2C_Init(XIic *IicInstancePtr, u16 DeviceId) {
    int Status;
    
    XIic_Config *ConfigPtr = XIic_LookupConfig(DeviceId);
    if (ConfigPtr == NULL) {
        return XST_FAILURE;
    }
    
    Status = XIic_CfgInitialize(IicInstancePtr, ConfigPtr, ConfigPtr->BaseAddress);
    if (Status != XST_SUCCESS) {
        return Status;
    }
    
    XIic_SetAddress(IicInstancePtr, XII_ADDR_TO_SEND_TYPE, GNSS_I2C_ADDRESS);
    
    XIic_SetSClk(IicInstancePtr, IIC_SCLK_RATE);
    
    return XST_SUCCESS;
}

int I2C_ReadRegister(u8 RegAddress, u8 *ReadBuffer, int ByteCount) {
    int Status;
    Status = XIic_Send(IicInstance.BaseAddress, GNSS_I2C_ADDRESS, &RegAddress, 1, XIIC_REPEATED_START);
    if (Status < 0) return Status;
    return XIic_Recv(IicInstance.BaseAddress, GNSS_I2C_ADDRESS, ReadBuffer, ByteCount, XIIC_STOP);
}

int I2C_ReadAvailableBytes() {
    u8 AvailableBytes = 0;
    I2C_ReadRegister(I2C_REG_DATA_READY, &AvailableBytes, 1);
    return AvailableBytes;
}

int I2C_ReadData(u8 *ReadBuffer, int ByteCount) {
    return I2C_ReadRegister(I2C_REG_DATA, ReadBuffer, ByteCount);
}

void SendToNUMAParser(u8 *Data, int Length) {
    for (int i = 0; i < Length; i++) {
        Xil_Out8(AXI_NUMA_CMD, Data[i]);
    }
}

void RetrieveParsedNUMAData() {
    if (Xil_In32(AXI_NUMA_STATUS) == 1) {
        int latitude = Xil_In32(AXI_NUMA_LAT);
        int longitude = Xil_In32(AXI_NUMA_LON);
        int timestamp = Xil_In32(AXI_NUMA_TIME);
        xil_printf("Parsed NUMA Data: Lat: %d, Lon: %d, Time: %d\n", latitude, longitude, timestamp);
    } else {
        xil_printf("NUMA Parser not ready\n");
    }
}

int main() {
    int Status;
    u8 RxBuffer[10];
    
    Status = I2C_Init(&IicInstance, IIC_DEVICE_ID);
    if (Status != XST_SUCCESS) {
        xil_printf("I2C Initialization Failed\n");
        return XST_FAILURE;
    }
    
    int AvailableBytes = I2C_ReadAvailableBytes();
    if (AvailableBytes <= 0) {
        xil_printf("No data available\n");
        return XST_FAILURE;
    }
    
    Status = I2C_ReadData(RxBuffer, (AvailableBytes < 10) ? AvailableBytes : 10);
    if (Status < 0) {
        xil_printf("I2C Read Failed\n");
        return XST_FAILURE;
    }
    
    SendToNUMAParser(RxBuffer, (AvailableBytes < 10) ? AvailableBytes : 10);
    RetrieveParsedNUMAData();
    
    return XST_SUCCESS;
}
