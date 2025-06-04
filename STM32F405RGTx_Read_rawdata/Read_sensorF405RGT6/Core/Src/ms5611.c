/*
 * ms5611.c
 *
 *  Created on: Oct 16, 2023
 *      Author: Admin
 */
#include "ms5611.h"
#include <math.h>

// PROM Calibration Coefficients
uint16_t C[7];

// I2C Handle
extern I2C_HandleTypeDef hi2c1;

double pres2alti(double pressure)
{
   return (1.0f - pow(pressure / 101325.0f, 0.190295f)) * 4433000.0f / 100; // meters
}

void MS5611_Init(void) {
    uint8_t cmd = CMD_RESET;
    HAL_I2C_Master_Transmit(&hi2c1, MS5611_ADDR, &cmd, 1, HAL_MAX_DELAY);
    HAL_Delay(10);  // Wait for reset
    MS5611_ReadPROM();  // Load calibration coefficients
}

uint32_t MS5611_ReadADC(void) {
    uint8_t data[3];
	HAL_I2C_Mem_Read(&hi2c1, MS5611_ADDR, CMD_ADC_READ, 1, data, 3, HAL_MAX_DELAY);
	return data[0] << 16 | data[1] << 8 | data[2];
}

void MS5611_ReadPROM(void) {
    for (uint8_t i = 0; i <= 6; i++) {
        uint8_t cmd = CMD_PROM_READ + (i * 2);
        uint8_t data[2];
        HAL_I2C_Mem_Read(&hi2c1, MS5611_ADDR, cmd, 1, data, 2, HAL_MAX_DELAY);
        C[i] = (data[0] << 8) | data[1];
    }
}

void MS5611_Calculate(double* pressure, double* temperature) {
    uint32_t D1, D2;
    int64_t dT, OFF, SENS;
    float temp, pres;
    float temp2 = 0.0f, OFF2 = 0.0f, SENS2 = 0.0f;
    float AUX;

    // Trigger and read D1 (Pressure)
    uint8_t cmd = CMD_CONV_D1;
    HAL_I2C_Master_Transmit(&hi2c1, MS5611_ADDR, &cmd, 1, HAL_MAX_DELAY);
    HAL_Delay(10);  // Max conversion time for OSR = 4096
    D1 = MS5611_ReadADC();

    // Trigger and read D2 (Temperature)
    cmd = CMD_CONV_D2;
    HAL_I2C_Master_Transmit(&hi2c1, MS5611_ADDR, &cmd, 1, HAL_MAX_DELAY);
    HAL_Delay(10);  // Max conversion time for OSR = 4096
    D2 = MS5611_ReadADC();

    // Calculate temperature
    dT = D2 - ((uint32_t)C[5] << 8);
    temp = 2000 + dT * ((uint32_t)C[6]) / 8388608.0f;

    // Calculate pressure offset and sensitivity
    OFF = (uint32_t)(C[2] << 16) + ((uint32_t)C[4] * dT) / 128.0f;
    SENS = (uint32_t)(C[1] << 15) + ((uint32_t)C[3] * dT) / 256.0f;

    // Apply second-order compensation
    if (temp < 2000) {
        AUX = (temp - 2000) * (temp - 2000);
        temp2 = (dT * dT) / 0x80000000;
        OFF2 = 2.5f * AUX;
        SENS2 = 1.25f * AUX;

        if (temp < -1500) {
            AUX = (temp + 1500) * (temp + 1500);
            OFF2 += 7.0f * AUX;
            SENS2 += 5.5f * AUX;
        }
    }

    temp -= temp2;
    OFF -= OFF2;
    SENS -= SENS2;

    // Calculate pressure
    pres = (D1 * SENS / 2097152.0f - OFF) / 32768.0f;

    // Return results
    *pressure = pres;        // Pressure in hPa
    *temperature = temp / 100.0f;  // Temperature in °C
}
