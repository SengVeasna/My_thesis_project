/*
 * Company: UAVx Co., LTD
 * Author1: HOM Ponlok
 * Author2: SENG Veasna
 * File: ekf10filter.h
 *
 * Date  : 21-May-2025 16:12:11
 */

#ifndef EKF10FILTER_H
#define EKF10FILTER_H

/* Include Files */
#include "ekf10filter_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void ekf10Init(double freq, const double initState[6], sysInit_t *sys);

extern void ekf10Step(sysInit_t *sys, const imuData_t *imu,
                      const double magNED[3], double alti);

extern void ekf10filter_initialize(void);

extern void ekf10filter_terminate(void);

extern void initStateEKF(const double meanAccel[3], const double meanMag[3],
                         double meanAlti, double initState[6]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for ekf10filter.h
 *
 * [EOF]
 */
