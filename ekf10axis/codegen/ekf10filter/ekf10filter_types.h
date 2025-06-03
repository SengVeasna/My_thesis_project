/*
 * Company: UAVx Co., LTD
 * Author1: HOM Ponlok
 * Author2: SENG Veasna
 * File: ekf10filter_types.h
 *
 * Date  : 21-May-2025 16:12:11
 */

#ifndef EKF10FILTER_TYPES_H
#define EKF10FILTER_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_sysInit_t
#define typedef_sysInit_t
typedef struct {
  double euler[3];
  double dt;
  double Q[6][6];
  double R_acc[3][3];
  double R_mag[3][3];
  double R_baro;
  double x[6];
  double P[6][6];
} sysInit_t;
#endif /* typedef_sysInit_t */

#ifndef typedef_imuData_t
#define typedef_imuData_t
typedef struct {
  double gyro[3];
  double accel[3];
} imuData_t;
#endif /* typedef_imuData_t */

#endif
/*
 * File trailer for ekf10filter_types.h
 *
 * [EOF]
 */
