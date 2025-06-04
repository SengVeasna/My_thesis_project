/*
 * sm5611.h
 *
 *  Created on: Oct 16, 2023
 *      Author: Admin
 */

#ifndef INC_MS5611_H_
#define INC_MS5611_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdint.h>
#include <math.h>

// Define MS5611 I2C Address
#define MS5611_ADDR 0x77 << 1  // Change to 0x76 << 1 if CSB pin is connected to GND

// MS5611 Commands
#define CMD_RESET 0x1E
#define CMD_PROM_READ 0xA0
#define CMD_CONV_D1 0x48  // Pressure conversion (OSR = 4096)
#define CMD_CONV_D2 0x58  // Temperature conversion (OSR = 4096)
#define CMD_ADC_READ 0x00

// Function Prototypes
extern void MS5611_Init(void);
extern uint32_t MS5611_ReadADC(void);
extern void MS5611_ReadPROM(void);
extern void MS5611_Calculate(double* pressure, double* temperature);
extern double pres2alti(double pressure);

#endif /* INC_MS5611_H_ */
