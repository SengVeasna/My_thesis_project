/*
 * File: _coder_ekf10Init_mex.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 11:43:49
 */

#ifndef _CODER_EKF10INIT_MEX_H
#define _CODER_EKF10INIT_MEX_H

/* Include Files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[],
                                     int32_T nrhs, const mxArray *prhs[]);

emlrtCTX mexFunctionCreateRootTLS(void);

void unsafe_ekf10Init_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                                  const mxArray *prhs[2]);

void unsafe_ekf10Step_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                                  const mxArray *prhs[4]);

void unsafe_initStateEKF_mexFunction(int32_T nlhs, mxArray *plhs[1],
                                     int32_T nrhs, const mxArray *prhs[3]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_ekf10Init_mex.h
 *
 * [EOF]
 */
