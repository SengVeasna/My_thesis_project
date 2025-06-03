/*
 * mpu6500.c
 *
 *  Created on: Nov 19, 2024
 *      Author: UAVxLab
 */

#include "mpu_6500.h"
#include <math.h>

extern SPI_HandleTypeDef hspi1;
// SPI Communication Functions
void MPU_Wirte_Register(uint8_t reg, uint8_t value) {
    uint8_t buf[2];
    buf[0] = reg & ~(0x80);  // Clear the read/write bit
    buf[1] = value;
    MPU6500_CS_LOW();
    HAL_SPI_Transmit(&hspi1, buf, 2, HAL_MAX_DELAY);
    MPU6500_CS_HIGH();
}
void MPU_Read_Register(uint8_t reg, uint8_t *buffer, uint8_t len) {
    uint8_t buf = reg | 0x80;  // Set the read bit
    MPU6500_CS_LOW();
    HAL_SPI_Transmit(&hspi1, &buf, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, buffer, len, HAL_MAX_DELAY);
    MPU6500_CS_HIGH();
}


// Read raw accelerometer and gyroscope data
void MPU6500_Read(int16_t gyroADC[3], int16_t accelADC[3]) {

    uint8_t buffer[6];

    // Read gyroscope data
    MPU_Read_Register(0x43, buffer, 6);
	gyroADC[1] = (buffer[0] << 8) | buffer[1];
	gyroADC[0] = (buffer[2] << 8) | buffer[3];
	gyroADC[2] = -((buffer[4] << 8) | buffer[5]);

    // Read accelerometer data
    MPU_Read_Register(0x3B, buffer, 6);

    accelADC[1] = -((buffer[0] << 8) | buffer[1]);
    accelADC[0] = -((buffer[2] << 8) | buffer[3]);
    accelADC[2] = ((buffer[4] << 8) | buffer[5]);

//    accelADC[0] = (buffer[0] << 8) | buffer[1];
//    accelADC[1] = (buffer[2] << 8) | buffer[3];
//    accelADC[2] = -((buffer[4] << 8) | buffer[5]);
}
// MPU-6500 Initialization
void MPU6500_Init(void) {
    // Wake up the MPU-6500
    MPU_Wirte_Register(0x6B, 0x00);

    // Set accelerometer range to ±8g
    MPU_Wirte_Register(0x1C, 0x10);

    // Set gyroscope range to ±1000 degrees/second
    MPU_Wirte_Register(0x1B, 0x10);

    // Set gyroscope DLPF to 41 Hz
    MPU_Wirte_Register(0x1A, 0x03);

    // Set accelerometer DLPF to 41 Hz
    MPU_Wirte_Register(0x1D, 0x03);
}

void MPU6500_Read_Scaled(double accel[3], double gyro[3], double meanAccel[3], double meanGyro[3]) {
	static int16_t gyroADC[3];
	static int16_t accelADC[3];
    MPU6500_Read(gyroADC, accelADC);
    for (int i=0; i<3; i++){
        // Convert accelerometer raw values to g
        accel[i] = (double)accelADC[i] / ACCEL_SENSITIVITY * 9.81 - meanAccel[i];
    	gyro[i] = (double)gyroADC[i] / GYRO_SENSITIVITY * (M_PI / 180.0) - meanGyro[i];
    }
}

