/*
 * File: _coder_ekf10Init_api.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 11:43:49
 */

#ifndef _CODER_EKF10INIT_API_H
#define _CODER_EKF10INIT_API_H

/* Include Files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <string.h>

/* Type Definitions */
#ifndef typedef_snaData_t
#define typedef_snaData_t
typedef struct {
  real_T euler[3];
  real_T dt;
  real_T Q[6][6];
  real_T R_acc[3][3];
  real_T R_mag[3][3];
  real_T R_baro;
  real_T x[6];
  real_T P[6][6];
} snaData_t;
#endif /* typedef_snaData_t */

#ifndef typedef_imuData_t
#define typedef_imuData_t
typedef struct {
  real_T gyro[3];
  real_T accel[3];
} imuData_t;
#endif /* typedef_imuData_t */

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void ekf10Init(real_T freq, real_T initState[6], snaData_t *sys);

void ekf10Init_api(const mxArray *const prhs[2], const mxArray **plhs);

void ekf10Init_atexit(void);

void ekf10Init_initialize(void);

void ekf10Init_terminate(void);

void ekf10Init_xil_shutdown(void);

void ekf10Init_xil_terminate(void);

void ekf10Step(snaData_t *sys, imuData_t *imu, real_T magNED[3], real_T alti);

void ekf10Step_api(const mxArray *const prhs[4], const mxArray **plhs);

void initStateEKF(real_T meanAccel[3], real_T meanMag[3], real_T meanAlti,
                  real_T initState[6]);

void initStateEKF_api(const mxArray *const prhs[3], const mxArray **plhs);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_ekf10Init_api.h
 *
 * [EOF]
 */
