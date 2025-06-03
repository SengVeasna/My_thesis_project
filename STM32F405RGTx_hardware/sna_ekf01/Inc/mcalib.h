/*
 * File: mcalib.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 22-Apr-2025 10:56:19
 */

#ifndef MCALIB_H
#define MCALIB_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void mcalib(const double rawMag[3], double CorrectMag[3]);

extern void mcalib_initialize(void);

extern void mcalib_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for mcalib.h
 *
 * [EOF]
 */
