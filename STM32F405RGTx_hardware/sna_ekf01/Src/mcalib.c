/*
 * File: mcalib.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 22-Apr-2025 10:56:19
 */

/* Include Files */
#include "mcalib.h"

/* Function Definitions */
/*
 * Arguments    : const double rawMag[3]
 *                double CorrectMag[3]
 * Return Type  : void
 */
void mcalib(const double rawMag[3], double CorrectMag[3])
{
//  static const double b[3][3] = {{0.944833187002975,   0.008526001259189,   0.021582394549387},
//                                 {0.008526001259189,   0.942625800789117,  -0.022986087070788},
//                                 {0.021582394549387,  -0.022986087070788,   1.123962854822809}};

  static const double b[3][3] = {{1,     0,     0},
                                 {0,     1,     0},
                                 {0,     0,     1}};

  double rawMag_idx_0;
  double rawMag_idx_1;
  double rawMag_idx_2;
  int i;
  rawMag_idx_0 = rawMag[0] - 6.386843848016684;
  rawMag_idx_1 = rawMag[1] + 38.816937330020657;
  rawMag_idx_2 = rawMag[2] - 11.246602136081217;
  for (i = 0; i < 3; i++) {
    CorrectMag[i] = (rawMag_idx_0 * b[i][0] + rawMag_idx_1 * b[i][1]) +
                    rawMag_idx_2 * b[i][2];
  }
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void mcalib_initialize(void)
{
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void mcalib_terminate(void)
{
}

/*
 * File trailer for mcalib.c
 *
 * [EOF]
 */
