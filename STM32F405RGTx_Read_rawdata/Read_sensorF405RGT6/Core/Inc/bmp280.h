/*
 * bmp280.h
 *
 *  Created on: Oct 11, 2023
 *      Author: Admin
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_
#include "main.h"
#include "stdbool.h"

#define BMP280_I2CADDR			0x76 << 1

#define	BMP280_CHIPID			0xD0
#define	BMP280_STATUS			0xF3
#define	BMP280_CONTROL			0xF4
#define	BMP280_CONFIG			0xF5
#define	BMP280_PRESSUREDATA		0xF7
#define	BMP280_TEMPDATA			0xFA

#define	BMP280_DIG_T1			0x88
#define	BMP280_DIG_T2			0x8A
#define	BMP280_DIG_T3			0x8C
#define	BMP280_DIG_P1			0x8E
#define	BMP280_DIG_P2			0x90
#define	BMP280_DIG_P3			0x92
#define	BMP280_DIG_P4			0x94
#define	BMP280_DIG_P5			0x96
#define	BMP280_DIG_P6			0x98
#define	BMP280_DIG_P7			0x9A
#define	BMP280_DIG_P8			0x9C
#define	BMP280_DIG_P9			0x9E

// Temperature resolution
#define BMP280_TEMPERATURE_16BIT 1
#define BMP280_TEMPERATURE_17BIT 2
#define BMP280_TEMPERATURE_18BIT 3
#define BMP280_TEMPERATURE_19BIT 4
#define BMP280_TEMPERATURE_20BIT 5

// Pressure oversampling
#define BMP280_ULTRALOWPOWER	1
#define BMP280_LOWPOWER			2
#define BMP280_STANDARD			3
#define BMP280_HIGHRES			4
#define BMP280_ULTRAHIGHRES		5

// Mode
#define BMP280_SLEEPMODE		0
#define BMP280_FORCEDMODE		1
#define BMP280_NORMALMODE		3

// IIR filter
#define BME280_FILTER_OFF		0
#define BME280_FILTER_X2 		1
#define BME280_FILTER_X4 		2
#define BME280_FILTER_X8		3
#define BME280_FILTER_X16 		4

#define	BMP280_MEASURING	    (1<<3)

#define CALIBRATING_BARO_TIME_MS	2000

typedef struct{

	int32_t t_fine;
	int32_t bmp280_up;
	int32_t bmp280_ut;

	int32_t baroPressure;
	int32_t baroTemperature;
	int32_t BaroAlt;

}baro_t;

typedef enum {
    BAROMETER_NEEDS_SAMPLES = 0,
    BAROMETER_NEEDS_CALCULATION
} barometerState_e;

//extern baro_t Baro;
bool bmp280_Init();
bool bmp280_StartUT();
bool bmp280_StartUP();
void bmp280_GetUP();
void bmp280_GetUT();
int32_t bmp280_CompensateTemperature(int32_t adc_T);
int32_t bmp280_CompensatePressure(int32_t adc_P);
void bmp280_Update();
void baroStartCalibration(void);
bool baroIsCalibrationComplete(void);
int32_t baroCalculateAltitude(void);
extern double pressureToAltitude(double pressure, double offsetAlti);

#endif /* INC_BMP280_H_ */
