/*
 * File: main.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 11:43:49
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

/* Include Files */
#include "main.h"
#include "ekf10Init.h"
#include "ekf10Init_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Declarations */
static void argInit_1x3_real_T(double result[3]);

static void argInit_3x3_real_T(double result[3][3]);

static void argInit_6x1_real_T(double result[6]);

static void argInit_6x6_real_T(double result[6][6]);

static imuData_t argInit_imuData_t(void);

static double argInit_real_T(void);

static void argInit_snaData_t(snaData_t *result);

/* Function Definitions */
/*
 * Arguments    : double result[3]
 * Return Type  : void
 */
static void argInit_1x3_real_T(double result[3])
{
  int idx1;
  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 3; idx1++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx1] = argInit_real_T();
  }
}

/*
 * Arguments    : double result[3][3]
 * Return Type  : void
 */
static void argInit_3x3_real_T(double result[3][3])
{
  int idx0;
  int idx1;
  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 3; idx0++) {
    for (idx1 = 0; idx1 < 3; idx1++) {
      /* Set the value of the array element.
Change this value to the value that the application requires. */
      result[idx1][idx0] = argInit_real_T();
    }
  }
}

/*
 * Arguments    : double result[6]
 * Return Type  : void
 */
static void argInit_6x1_real_T(double result[6])
{
  int idx0;
  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 6; idx0++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx0] = argInit_real_T();
  }
}

/*
 * Arguments    : double result[6][6]
 * Return Type  : void
 */
static void argInit_6x6_real_T(double result[6][6])
{
  int idx0;
  int idx1;
  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 6; idx0++) {
    for (idx1 = 0; idx1 < 6; idx1++) {
      /* Set the value of the array element.
Change this value to the value that the application requires. */
      result[idx1][idx0] = argInit_real_T();
    }
  }
}

/*
 * Arguments    : void
 * Return Type  : imuData_t
 */
static imuData_t argInit_imuData_t(void)
{
  imuData_t result;
  /* Set the value of each structure field.
Change this value to the value that the application requires. */
  argInit_1x3_real_T(result.gyro);
  result.accel[0] = result.gyro[0];
  result.accel[1] = result.gyro[1];
  result.accel[2] = result.gyro[2];
  return result;
}

/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}

/*
 * Arguments    : snaData_t *result
 * Return Type  : void
 */
static void argInit_snaData_t(snaData_t *result)
{
  double result_tmp;
  int i;
  /* Set the value of each structure field.
Change this value to the value that the application requires. */
  argInit_3x3_real_T(result->R_acc);
  result_tmp = argInit_real_T();
  argInit_6x6_real_T(result->Q);
  argInit_1x3_real_T(result->euler);
  result->dt = result_tmp;
  result->R_baro = result_tmp;
  argInit_6x1_real_T(result->x);
  for (i = 0; i < 3; i++) {
    result->R_mag[i][0] = result->R_acc[i][0];
    result->R_mag[i][1] = result->R_acc[i][1];
    result->R_mag[i][2] = result->R_acc[i][2];
  }
  memcpy(&result->P[0][0], &result->Q[0][0], 36U * sizeof(double));
}

/*
 * Arguments    : int argc
 *                char **argv
 * Return Type  : int
 */
int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  /* Initialize the application.
You do not need to do this more than one time. */
  ekf10Init_initialize();
  /* Invoke the entry-point functions.
You can call entry-point functions multiple times. */
  main_ekf10Init();
  main_ekf10Step();
  main_initStateEKF();
  /* Terminate the application.
You do not need to do this more than one time. */
  ekf10Init_terminate();
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_ekf10Init(void)
{
  snaData_t sys;
  double dv[6];
  /* Initialize function 'ekf10Init' input arguments. */
  /* Initialize function input argument 'initState'. */
  /* Call the entry-point 'ekf10Init'. */
  argInit_6x1_real_T(dv);
  ekf10Init(argInit_real_T(), dv, &sys);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_ekf10Step(void)
{
  imuData_t r;
  snaData_t sys;
  double dv[3];
  /* Initialize function 'ekf10Step' input arguments. */
  /* Initialize function input argument 'sys'. */
  /* Initialize function input argument 'imu'. */
  /* Initialize function input argument 'magNED'. */
  /* Call the entry-point 'ekf10Step'. */
  argInit_snaData_t(&sys);
  argInit_1x3_real_T(dv);
  r = argInit_imuData_t();
  ekf10Step(&sys, &r, dv, argInit_real_T());
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_initStateEKF(void)
{
  double initState[6];
  double meanAccel_tmp[3];
  /* Initialize function 'initStateEKF' input arguments. */
  /* Initialize function input argument 'meanAccel'. */
  argInit_1x3_real_T(meanAccel_tmp);
  /* Initialize function input argument 'meanMag'. */
  /* Call the entry-point 'initStateEKF'. */
  initStateEKF(meanAccel_tmp, meanAccel_tmp, argInit_real_T(), initState);
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
