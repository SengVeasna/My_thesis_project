/*
 * QMC5883P.c
 *
 *  Created on: Aug 14, 2023
 *      Author: UAVxLab
 */
#include "QMC5883P.h"

static void qmc5883p_Write_Reg(uint8_t reg, uint8_t value){
	uint8_t buf = value;
	HAL_I2C_Mem_Write(&hi2c2, Q5883P_ADDR, reg, 1, &buf, 1, HAL_MAX_DELAY);

}
static void qmc5883p_Read_Reg(uint8_t reg, uint8_t *buffer, uint8_t len){

	HAL_I2C_Mem_Read(&hi2c2, Q5883P_ADDR, reg, 1, buffer, len, HAL_MAX_DELAY);

}
uint8_t qmc5883p_init(){

	uint8_t set_osr = 0xCD;		// set OSR to 200Hz
	uint8_t set_range = 0x08;	// set full range to 8 Guass
	uint8_t qmc_ID;
	qmc5883p_Read_Reg(Q5883P_ID_ADDR, &qmc_ID, 1);

	if ( qmc_ID != 0x80 ) return 0;

	qmc5883p_Write_Reg(0x29, 0x06);
	HAL_Delay(50);
	qmc5883p_Write_Reg(Q5883P_CONTR_2, 0x40);
	HAL_Delay(50);
	qmc5883p_Write_Reg(Q5883P_CONTR_2, set_range);
	HAL_Delay(50);
	qmc5883p_Write_Reg(Q5883P_CONTR_1, set_osr);
	HAL_Delay(50);
	return 1;

}
uint8_t qmc5883p_read(double magWorld[3]){

	uint8_t qmc_data_status;
	uint8_t buf[6];
	int16_t magraw[3];

	qmc5883p_Read_Reg(Q5883P_DATA_READY, &qmc_data_status, 1);

	if ( (qmc_data_status & 0x01) != 1 ) {

		return 0;
	}

	qmc5883p_Read_Reg(Q5883P_DATA_REG, buf, 6);

	magraw[1] = -(buf[1] << 8 | buf[0]);
	magraw[0] = -(buf[3] << 8 | buf[2]);
	magraw[2] = -(buf[5] << 8 | buf[4]);


//	magraw[0] = -magraw[1];
//	magraw[1] = -magraw[0];
//	magraw[2] = -magraw[2];
//	magraw[0] = -magraw[1];
//	magraw[1] = magraw[0];

	for (int i=0; i<3; i++) magWorld[i] = (magraw[i] * 0.026666666666667); //ADC_value / Sensitivity * 100

	return 1;
}
