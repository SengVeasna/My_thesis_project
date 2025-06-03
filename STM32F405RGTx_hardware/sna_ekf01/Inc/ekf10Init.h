/*
 * File: ekf10Init.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 11:43:49
 */

#ifndef EKF10INIT_H
#define EKF10INIT_H

/* Include Files */
#include "ekf10Init_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void ekf10Init(double freq, const double initState[6], snaData_t *sys);

extern void ekf10Init_initialize(void);

extern void ekf10Init_terminate(void);

extern void ekf10Step(snaData_t *sys, const imuData_t *imu,
                      const double magNED[3], double alti);

extern void initStateEKF(const double meanAccel[3], const double meanMag[3],
                         double meanAlti, double initState[6]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for ekf10Init.h
 *
 * [EOF]
 */
