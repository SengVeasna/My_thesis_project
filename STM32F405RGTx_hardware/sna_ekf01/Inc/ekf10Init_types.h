/*
 * File: ekf10Init_types.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 11:43:49
 */

#ifndef EKF10INIT_TYPES_H
#define EKF10INIT_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_snaData_t
#define typedef_snaData_t
typedef struct {
  double euler[3];
  double dt;
  double Q[6][6];
  double R_acc[3][3];
  double R_mag[3][3];
  double R_baro;
  double x[6];
  double P[6][6];
} snaData_t;
#endif /* typedef_snaData_t */

#ifndef typedef_imuData_t
#define typedef_imuData_t
typedef struct {
  double gyro[3];
  double accel[3];
} imuData_t;
#endif /* typedef_imuData_t */

#endif
/*
 * File trailer for ekf10Init_types.h
 *
 * [EOF]
 */
