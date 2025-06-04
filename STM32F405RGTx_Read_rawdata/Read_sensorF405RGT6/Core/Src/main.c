/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu_6500.h"
#include "QMC5883P.h"
#include "bmp280.h"
#include <stdbool.h>
#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

double meanAccel[3];
double meanGyro[3];
double meanMag[3];
double meanAlti;

double rawMag[3];
double rawAccel[3];
double rawGyro[3];

double rawAlti;
extern baro_t Baro;

/* Variable Definitions */

/* Variable Definitions */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void sensorCalib(void){
	double AltiTmp = 0;
	double mean_tmp[3];
	double accel[3];
	double gyro[3];
	double mag[3];
	for (int i = 0; i < 100; i++) {
		bmp280_Update();
		AltiTmp = pressureToAltitude(Baro.baroPressure, 0);
		meanAlti += AltiTmp;
		qmc5883p_read(mag);
		MPU6500_Read_Scaled(accel, gyro, mean_tmp, mean_tmp);
//    rawAltiTmp = (1.0f - pow(Baro.baroPressure / 101325.0f, 0.190295f)) * 44330.0f - 0; //OffsetAlti = 0
//    alti = pressureToAltitude(Baro.baroPressure, offsettmp);
//    rawAltiTmp = (1.0f - powf(Baro.baroPressure / 101325.0f, 0.190267f)) * 44330.0f;
//    meanAlti += rawAltiTmp;
		for (int i = 0; i < 3; i++) {
			meanGyro[i] += gyro[i];
			meanMag[i] += mag[i];
			meanAccel[i] += accel[i];
		}
//		meanAccel[0] += accel[0];
//		meanAccel[1] += accel[1];
//    	meanAlti += rawAltiTmp;
		HAL_Delay(10);
	}
	for (int i = 0; i < 3; i++) {
		meanGyro[i] /= 100;
		meanMag[i] /= 100;
		meanAccel[i] /= 100;
	}
	meanAlti /= 100;
//	meanAccel[0] /= 100;
//	meanAccel[1] /= 100;
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) { //Timer2 100Hz
    qmc5883p_read(rawMag);
    MPU6500_Read_Scaled(rawAccel, rawGyro, meanAccel, meanGyro);
  }
  if (htim->Instance == TIM3) { //Timer3 50Hz
    bmp280_Update();
    /*Read Raw altitude from pressure with library*/
    rawAlti = pressureToAltitude(Baro.baroPressure, meanAlti);
  }
}

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//float temperature,altitude;

//int32_t pressure;
//FIRFilter FIRFilter_pressure;
//IIRFilter Filter_pressure;
//float rawAlti;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//  if (htim->Instance == TIM2) { //Timer2 100Hz
//    qmc5883p_read(rawMag);
//    MPU6500_Read_Scaled(rawAccel, rawGyro, meanAccel, meanGyro);
//  }
//  if (htim->Instance == TIM3) { //Timer3 50Hz
//    bmp280_Update();
////    rawAlti = pressureToAltitude(Baro.baroPressure, meanAlti);
////    Alti = (1.0f - pow(Baro.baroPressure / 101325.0f, 0.190295f)) * 44330.0f; //OffsetAlti = 0
////    rawAlti = Alti - meanAlti;
//    /*Read Raw altitude from pressure with library*/
//    rawAlti = pressureToAltitude(Baro.baroPressure, meanAlti);
////    rawAlti = Alti - meanAlti;
//  }
//}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  //Init sensor before calibrate
  MPU6500_Init();
  qmc5883p_init();
  bmp280_Init();
  sensorCalib();

  HAL_TIM_Base_Start_IT(&htim2); //Using timer2 100Hz
  HAL_TIM_Base_Start_IT(&htim3); //Using timer3 50Hz

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//    BMP280_ReadTemperatureAndPressure(&temperature, &pressure);
//    FIRFilter_Update(&FIRFilter_pressure, pressure);
//    IIRFilter_Update(&Filter_pressure, pressure);
//
////    altitude = 44330.0f * (1 - powf(Filter_pressure.out / 101025.0f, 1.0f / 5.255f));
//
//    rawAlti = 47253.84f * (1 - powf(Filter_pressure.out / 100950.0f, 1.0f / 5.255f)); //Use T0 = 34,35 Angsasel &P0 in Cambodia
////    altitude = 44330.0f * (1 - powf(pressure / 101325.0f, 1.0f / 5.255f));
////    altitude = 44330.0f * (1 - powf(pressure / 100900.0f, 1.0f / 5.255f));
////    altitude = 44330.0f * (1 - powf(pressure / 100750.0f, 1.0f / 5.255f));
////    rawAlti = 44330.0f * (1 - powf(Filter_pressure.out / 101025.0f, 1.0f / 5.255f));
//      bmp280_Update();
//      rawAlti = (1.0f - pow(Baro.baroPressure / 101325.0f, 0.190295f)) * 4433000.0f / 100.0  - 0; //OffsetAlti = 0
//    HAL_Delay(20);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
