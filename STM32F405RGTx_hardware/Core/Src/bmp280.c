/*
 * bmp280.c
 *
 *  Created on: Oct 11, 2023
 *      Author: UAVxLab
 */

#include "bmp280.h"
#include "math.h"


baro_t Baro;                        // barometer access functions

typedef struct{
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;

}bmp280_param_t;

bmp280_param_t bmp280_cal;
extern I2C_HandleTypeDef hi2c1;

uint8_t BMP280_MODE		=	(BMP280_TEMPERATURE_16BIT <<5 | BMP280_HIGHRES<<2 | BMP280_NORMALMODE);
uint8_t BMP280_FILTER 	=	(BME280_FILTER_X8 << 2);

static void bmp280_ReadCalibrateValue(){
	uint8_t buf[24];

	HAL_I2C_Mem_Read(&hi2c1, BMP280_I2CADDR, BMP280_DIG_T1, 1, buf, 24, 100);

	bmp280_cal.dig_T1 = buf[1] << 8 | buf[0];
	bmp280_cal.dig_T2 = buf[3] << 8 | buf[2];
	bmp280_cal.dig_T3 = buf[5] << 8 | buf[4];

	bmp280_cal.dig_P1 = buf[7] << 8 | buf[6];
	bmp280_cal.dig_P2 = buf[9] << 8 | buf[8];
	bmp280_cal.dig_P3 = buf[11]<< 8 | buf[10];
	bmp280_cal.dig_P4 = buf[13]<< 8 | buf[12];
	bmp280_cal.dig_P5 = buf[15]<< 8 | buf[14];
	bmp280_cal.dig_P6 = buf[17]<< 8 | buf[16];
	bmp280_cal.dig_P7 = buf[19]<< 8 | buf[18];
	bmp280_cal.dig_P8 = buf[21]<< 8 | buf[20];
	bmp280_cal.dig_P9 = buf[23]<< 8 | buf[22];
}
bool bmp280_Init() {
    uint8_t checkID;

    // Read chip ID
    HAL_I2C_Mem_Read(&hi2c1, BMP280_I2CADDR, BMP280_CHIPID, 1, &checkID, 1, 10);
    if (checkID != 0x58 && checkID != 0x60) return false; // Check for BMP280 or BME280

    // Configure sensor settings
    uint8_t config = (BME280_FILTER_X8 << 2); // Set IIR filter coefficient
    HAL_I2C_Mem_Write(&hi2c1, BMP280_I2CADDR, BMP280_CONFIG, 1, &config, 1, 10);

    uint8_t ctrl_meas = (BMP280_TEMPERATURE_16BIT << 5) | (BMP280_HIGHRES << 2) | BMP280_NORMALMODE;
    HAL_I2C_Mem_Write(&hi2c1, BMP280_I2CADDR, BMP280_CONTROL, 1, &ctrl_meas, 1, 10);

    // Read calibration data
    bmp280_ReadCalibrateValue();

    return true;
}

bool bmp280_StartUT(){

	return true;
}
void bmp280_GetUT(){
	// Do nothing
}
bool bmp280_StartUP(){

	return true;
}
void bmp280_GetUP(){

	uint8_t buf[6];
	HAL_I2C_Mem_Read(&hi2c1, BMP280_I2CADDR, BMP280_PRESSUREDATA, 1, buf, 6, 10);
	Baro.bmp280_up = (buf[0] << 12) | (buf[1] << 4) | buf[2] >> 4;
	Baro.bmp280_ut = (buf[3] << 12) | (buf[4] << 4) | buf[5] >> 4;


}
int32_t bmp280_CompensateTemperature(int32_t adc_T) {
    int32_t var1, var2, temp_adc, T;
    temp_adc = adc_T;

    var1 = ((((temp_adc >> 3) - ((int32_t)bmp280_cal.dig_T1 << 1))) * ((int32_t)bmp280_cal.dig_T2)) >> 11;
    var2 = (((((temp_adc >> 4) - ((int32_t)bmp280_cal.dig_T1)) * ((temp_adc >> 4) - ((int32_t)bmp280_cal.dig_T1))) >> 12) * ((int32_t)bmp280_cal.dig_T3)) >> 14;

    Baro.t_fine = (var1 + var2);

    T = (Baro.t_fine * 5 + 128) >> 8;

    return T;
}
int32_t bmp280_CompensatePressure(int32_t adc_P) {
    int32_t pres_adc;
    int64_t var1, var2, p;

    pres_adc = adc_P;

    var1 = ((int64_t)Baro.t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bmp280_cal.dig_P6;
    var2 = var2 + ((var1 * (int64_t)bmp280_cal.dig_P5) << 17);
    var2 = var2 + (((int64_t)bmp280_cal.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)bmp280_cal.dig_P3) >> 8) + ((var1 * (int64_t)bmp280_cal.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1) * ((int64_t)bmp280_cal.dig_P1)) >> 33;

    if (var1 == 0) return 0;

    p = 1048576 - pres_adc;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)bmp280_cal.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)bmp280_cal.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280_cal.dig_P7) << 4);
    return (int32_t)p;
}
static void bmp280_Calculate(int32_t *pressure, int32_t *temperature){
	int32_t t = bmp280_CompensateTemperature(Baro.bmp280_ut);
	int32_t p = bmp280_CompensatePressure(Baro.bmp280_up);

	if (pressure) *pressure = p / 256;
	if (temperature) * temperature = t;

}

double pressureToAltitude(double pressure, double offsetAlti)
{
   return (1.0f - pow(pressure / 101325.0f, 0.190295f)) * 4433000.0f / 100.0  - offsetAlti;
}

double altitudeToPressure(const double altCm)
{
    return powf(1.0f - (altCm / 4433000.0f), 5.254999) * 101325.0f;
}


void bmp280_Update(){
	bmp280_GetUP();
	bmp280_Calculate(&Baro.baroPressure, &Baro.baroTemperature);
}


