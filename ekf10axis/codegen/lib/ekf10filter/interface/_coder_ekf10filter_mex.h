/*
 * Company: UAVx Co., LTD
 * Author1: HOM Ponlok
 * Author2: SENG Veasna
 * File: _coder_ekf10filter_mex.h
 *
 * Date  : 21-May-2025 16:12:11
 */

#ifndef _CODER_EKF10FILTER_MEX_H
#define _CODER_EKF10FILTER_MEX_H

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
 * File trailer for _coder_ekf10filter_mex.h
 *
 * [EOF]
 */
