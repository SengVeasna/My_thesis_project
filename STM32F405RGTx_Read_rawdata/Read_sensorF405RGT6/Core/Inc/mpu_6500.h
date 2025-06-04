/*
 * mpu_6500.h
 *
 *  Created on: Nov 19, 2024
 *      Author: AI_Farm
 */

#ifndef INC_MPU_6500_H_
#define INC_MPU_6500_H_

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "gpio.h"

typedef struct{
	float x;
	float y;
	float z;
}axis_t;

typedef struct{
	axis_t gyro;			// rad/s
	axis_t accel;			// m/s2

	int16_t gyroADC[3];
	int16_t accelADC[3];

	uint16_t gyroOffset[3];
	uint16_t accelOffset[3];

}accelGyro_t;

#define MPU6500_CS_LOW()  HAL_GPIO_WritePin(CS_GPIO_GPIO_Port, CS_GPIO_Pin, GPIO_PIN_RESET)
#define MPU6500_CS_HIGH() HAL_GPIO_WritePin(CS_GPIO_GPIO_Port, CS_GPIO_Pin, GPIO_PIN_SET)

// Constants for sensitivity settings
#define ACCEL_SENSITIVITY 4096.0  // For ±8g range
#define GYRO_SENSITIVITY 32.8     // For ±1000 degrees/second range

extern uint8_t MPU6500_SPI_Read(uint8_t reg);
extern void MPU6500_SPI_Write(uint8_t reg, uint8_t data);
extern void MPU6500_Init(void);
extern void MPU6500_Read_Scaled(double accel[3], double gyro[3], double meanAccel[3], double meanGyro[3]);
#endif /* INC_MPU_6500_H_ */
