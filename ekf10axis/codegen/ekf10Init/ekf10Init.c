/*
 * File: ekf10Init.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 11:43:49
 */

/* Include Files */
#include "ekf10Init.h"
#include "ekf10Init_types.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static double b_atan2(double y, double x);

static void b_diag(const double v[3], double d[3][3]);

static void b_eye(double b_I[4][4]);

static double b_norm(const double x[3]);

static void blkdiag(double varargin_1[3][3], double varargin_2[3][3],
                    double varargin_3, double y[7][7]);

static double c_norm(const double x[2]);

static void diag(const double v[6], double d[6][6]);

static double ecompass(const double a[3], const double m[3], double *O_b,
                       double *O_c, double *O_d);

static void eye(double b_I[6][6]);

static void mrdiv(double A[7][6], double B[7][7]);

static void quat2eul(const double q[4], double eul[3]);

static void quat2rotm(const double q[4], double R[3][3]);

static void quatinv(double q[4]);

static void quatmultiply(const double q[4], const double r[4], double qout[4]);

static void quatnormalize(double q[4]);

static double rt_powd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double y
 *                double x
 * Return Type  : double
 */
static double b_atan2(double y, double x)
{
  double r;
  if (rtIsNaN(y) || rtIsNaN(x)) {
    r = rtNaN;
  } else if (rtIsInf(y) && rtIsInf(x)) {
    int i;
    int i1;
    if (y > 0.0) {
      i = 1;
    } else {
      i = -1;
    }
    if (x > 0.0) {
      i1 = 1;
    } else {
      i1 = -1;
    }
    r = atan2(i, i1);
  } else if (x == 0.0) {
    if (y > 0.0) {
      r = RT_PI / 2.0;
    } else if (y < 0.0) {
      r = -(RT_PI / 2.0);
    } else {
      r = 0.0;
    }
  } else {
    r = atan2(y, x);
  }
  return r;
}

/*
 * Arguments    : const double v[3]
 *                double d[3][3]
 * Return Type  : void
 */
static void b_diag(const double v[3], double d[3][3])
{
  int i;
  for (i = 0; i < 3; i++) {
    d[i][0] = 0.0;
    d[i][1] = 0.0;
    d[i][2] = 0.0;
  }
  d[0][0] = v[0];
  d[1][1] = v[1];
  d[2][2] = v[2];
}

/*
 * Arguments    : double b_I[4][4]
 * Return Type  : void
 */
static void b_eye(double b_I[4][4])
{
  int i;
  for (i = 0; i < 4; i++) {
    b_I[i][0] = 0.0;
    b_I[i][1] = 0.0;
    b_I[i][2] = 0.0;
    b_I[i][3] = 0.0;
  }
  b_I[0][0] = 1.0;
  b_I[1][1] = 1.0;
  b_I[2][2] = 1.0;
  b_I[3][3] = 1.0;
}

/*
 * Arguments    : const double x[3]
 * Return Type  : double
 */
static double b_norm(const double x[3])
{
  double absxk;
  double scale;
  double t;
  double y;
  scale = 3.3121686421112381E-170;
  absxk = fabs(x[0]);
  if (absxk > 3.3121686421112381E-170) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    y = t * t;
  }
  absxk = fabs(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }
  absxk = fabs(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }
  return scale * sqrt(y);
}

/*
 * Arguments    : double varargin_1[3][3]
 *                double varargin_2[3][3]
 *                double varargin_3
 *                double y[7][7]
 * Return Type  : void
 */
static void blkdiag(double varargin_1[3][3], double varargin_2[3][3],
                    double varargin_3, double y[7][7])
{
  int i;
  memset(&y[0][0], 0, 49U * sizeof(double));
  for (i = 0; i < 3; i++) {
    y[i][0] = varargin_1[i][0];
    y[i + 3][3] = varargin_2[i][0];
    y[i][1] = varargin_1[i][1];
    y[i + 3][4] = varargin_2[i][1];
    y[i][2] = varargin_1[i][2];
    y[i + 3][5] = varargin_2[i][2];
  }
  y[6][6] = varargin_3;
}

/*
 * Arguments    : const double x[2]
 * Return Type  : double
 */
static double c_norm(const double x[2])
{
  double absxk;
  double scale;
  double t;
  double y;
  scale = 3.3121686421112381E-170;
  absxk = fabs(x[0]);
  if (absxk > 3.3121686421112381E-170) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    y = t * t;
  }
  absxk = fabs(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }
  return scale * sqrt(y);
}

/*
 * Arguments    : const double v[6]
 *                double d[6][6]
 * Return Type  : void
 */
static void diag(const double v[6], double d[6][6])
{
  int j;
  memset(&d[0][0], 0, 36U * sizeof(double));
  for (j = 0; j < 6; j++) {
    d[j][j] = v[j];
  }
}

/*
 * Arguments    : const double a[3]
 *                const double m[3]
 *                double *O_b
 *                double *O_c
 *                double *O_d
 * Return Type  : double
 */
static double ecompass(const double a[3], const double m[3], double *O_b,
                       double *O_c, double *O_d)
{
  double R[3][3];
  double psquared[4];
  double Reast[3];
  double O_a;
  double pd;
  int b_k;
  int idx;
  int k;
  bool b[3][3];
  bool y[3];
  bool exitg1;
  bool nanPageIdx;
  Reast[0] = a[1] * m[2] - m[1] * a[2];
  Reast[1] = m[0] * a[2] - a[0] * m[2];
  Reast[2] = a[0] * m[1] - m[0] * a[1];
  R[2][0] = a[0];
  R[1][0] = Reast[0];
  R[2][1] = a[1];
  R[1][1] = Reast[1];
  R[2][2] = a[2];
  R[1][2] = Reast[2];
  R[0][0] = Reast[1] * a[2] - a[1] * Reast[2];
  R[0][1] = a[0] * Reast[2] - Reast[0] * a[2];
  R[0][2] = Reast[0] * a[1] - a[0] * Reast[1];
  for (k = 0; k < 3; k++) {
    double d;
    double d1;
    O_a = R[k][0];
    pd = R[k][1];
    d = R[k][2];
    d1 = sqrt((O_a * O_a + pd * pd) + d * d);
    O_a /= d1;
    R[k][0] = O_a;
    b[k][0] = rtIsNaN(O_a);
    O_a = pd / d1;
    R[k][1] = O_a;
    b[k][1] = rtIsNaN(O_a);
    O_a = d / d1;
    R[k][2] = O_a;
    b[k][2] = rtIsNaN(O_a);
  }
  for (k = 0; k < 3; k++) {
    y[k] = false;
    b_k = 0;
    exitg1 = false;
    while ((!exitg1) && (b_k < 3)) {
      if (b[k][b_k]) {
        y[k] = true;
        exitg1 = true;
      } else {
        b_k++;
      }
    }
  }
  nanPageIdx = false;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 3)) {
    if (y[k]) {
      nanPageIdx = true;
      exitg1 = true;
    } else {
      k++;
    }
  }
  if (nanPageIdx) {
    for (b_k = 0; b_k < 3; b_k++) {
      R[b_k][0] = 0.0;
      R[b_k][1] = 0.0;
      R[b_k][2] = 0.0;
    }
    R[0][0] = 1.0;
    R[1][1] = 1.0;
    R[2][2] = 1.0;
  }
  pd = (R[0][0] + R[1][1]) + R[2][2];
  psquared[0] = (2.0 * pd + 1.0) - pd;
  psquared[1] = (2.0 * R[0][0] + 1.0) - pd;
  psquared[2] = (2.0 * R[1][1] + 1.0) - pd;
  psquared[3] = (2.0 * R[2][2] + 1.0) - pd;
  if (!rtIsNaN(psquared[0])) {
    idx = 1;
  } else {
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 5)) {
      if (!rtIsNaN(psquared[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (idx == 0) {
    pd = psquared[0];
    idx = 1;
  } else {
    pd = psquared[idx - 1];
    b_k = idx + 1;
    for (k = b_k; k < 5; k++) {
      O_a = psquared[k - 1];
      if (pd < O_a) {
        pd = O_a;
        idx = k;
      }
    }
  }
  switch (idx) {
  case 1:
    pd = sqrt(pd);
    O_a = 0.5 * pd;
    pd = 0.5 / pd;
    *O_b = pd * (R[2][1] - R[1][2]);
    *O_c = pd * (R[0][2] - R[2][0]);
    *O_d = pd * (R[1][0] - R[0][1]);
    break;
  case 2:
    pd = sqrt(pd);
    *O_b = 0.5 * pd;
    pd = 0.5 / pd;
    O_a = pd * (R[2][1] - R[1][2]);
    *O_c = pd * (R[1][0] + R[0][1]);
    *O_d = pd * (R[2][0] + R[0][2]);
    break;
  case 3:
    pd = sqrt(pd);
    *O_c = 0.5 * pd;
    pd = 0.5 / pd;
    O_a = pd * (R[0][2] - R[2][0]);
    *O_b = pd * (R[1][0] + R[0][1]);
    *O_d = pd * (R[2][1] + R[1][2]);
    break;
  default:
    pd = sqrt(pd);
    *O_d = 0.5 * pd;
    pd = 0.5 / pd;
    O_a = pd * (R[1][0] - R[0][1]);
    *O_b = pd * (R[2][0] + R[0][2]);
    *O_c = pd * (R[2][1] + R[1][2]);
    break;
  }
  if (O_a < 0.0) {
    O_a = -O_a;
    *O_b = -*O_b;
    *O_c = -*O_c;
    *O_d = -*O_d;
  }
  return O_a;
}

/*
 * Arguments    : double b_I[6][6]
 * Return Type  : void
 */
static void eye(double b_I[6][6])
{
  int k;
  memset(&b_I[0][0], 0, 36U * sizeof(double));
  for (k = 0; k < 6; k++) {
    b_I[k][k] = 1.0;
  }
}

/*
 * Arguments    : double A[7][6]
 *                double B[7][7]
 * Return Type  : void
 */
static void mrdiv(double A[7][6], double B[7][7])
{
  double b_A[7][7];
  double smax;
  int b_i;
  int i;
  int i1;
  int j;
  int jA;
  int jBcol;
  int jp1j;
  int k;
  signed char ipiv[7];
  for (i = 0; i < 7; i++) {
    for (i1 = 0; i1 < 7; i1++) {
      b_A[i][i1] = B[i][i1];
    }
    ipiv[i] = (signed char)(i + 1);
  }
  for (j = 0; j < 6; j++) {
    int b_tmp;
    int mmj_tmp;
    mmj_tmp = 5 - j;
    b_tmp = j << 3;
    jp1j = b_tmp + 2;
    jA = 8 - j;
    jBcol = 0;
    smax = fabs((&b_A[0][0])[b_tmp]);
    for (k = 2; k < jA; k++) {
      double s;
      s = fabs((&b_A[0][0])[(b_tmp + k) - 1]);
      if (s > smax) {
        jBcol = k - 1;
        smax = s;
      }
    }
    if ((&b_A[0][0])[b_tmp + jBcol] != 0.0) {
      if (jBcol != 0) {
        jA = j + jBcol;
        ipiv[j] = (signed char)(jA + 1);
        for (k = 0; k < 7; k++) {
          jBcol = j + k * 7;
          smax = (&b_A[0][0])[jBcol];
          i = jA + k * 7;
          (&b_A[0][0])[jBcol] = (&b_A[0][0])[i];
          (&b_A[0][0])[i] = smax;
        }
      }
      i = (b_tmp - j) + 7;
      for (b_i = jp1j; b_i <= i; b_i++) {
        (&b_A[0][0])[b_i - 1] /= (&b_A[0][0])[b_tmp];
      }
    }
    jA = b_tmp;
    for (jBcol = 0; jBcol <= mmj_tmp; jBcol++) {
      smax = (&b_A[0][0])[(b_tmp + jBcol * 7) + 7];
      if (smax != 0.0) {
        i = jA + 9;
        i1 = (jA - j) + 14;
        for (jp1j = i; jp1j <= i1; jp1j++) {
          (&b_A[0][0])[jp1j - 1] +=
              (&b_A[0][0])[((b_tmp + jp1j) - jA) - 8] * -smax;
        }
      }
      jA += 7;
    }
  }
  for (j = 0; j < 7; j++) {
    jBcol = 6 * j - 1;
    jA = 7 * j;
    for (k = 0; k < j; k++) {
      jp1j = 6 * k;
      smax = (&b_A[0][0])[k + jA];
      if (smax != 0.0) {
        for (b_i = 0; b_i < 6; b_i++) {
          i = (b_i + jBcol) + 1;
          (&A[0][0])[i] -= smax * (&A[0][0])[b_i + jp1j];
        }
      }
    }
    smax = 1.0 / (&b_A[0][0])[j + jA];
    for (b_i = 0; b_i < 6; b_i++) {
      i = (b_i + jBcol) + 1;
      (&A[0][0])[i] *= smax;
    }
  }
  for (j = 6; j >= 0; j--) {
    jBcol = 6 * j - 1;
    jA = 7 * j - 1;
    i = j + 2;
    for (k = i; k < 8; k++) {
      jp1j = 6 * (k - 1);
      smax = (&b_A[0][0])[k + jA];
      if (smax != 0.0) {
        for (b_i = 0; b_i < 6; b_i++) {
          i1 = (b_i + jBcol) + 1;
          (&A[0][0])[i1] -= smax * (&A[0][0])[b_i + jp1j];
        }
      }
    }
  }
  for (j = 5; j >= 0; j--) {
    signed char i2;
    i2 = ipiv[j];
    if (i2 != j + 1) {
      for (b_i = 0; b_i < 6; b_i++) {
        smax = A[j][b_i];
        A[j][b_i] = A[i2 - 1][b_i];
        A[i2 - 1][b_i] = smax;
      }
    }
  }
}

/*
 * Arguments    : const double q[4]
 *                double eul[3]
 * Return Type  : void
 */
static void quat2eul(const double q[4], double eul[3])
{
  double a;
  double b;
  double c;
  double unnamed_idx_0_tmp;
  double z1_idx_0;
  double z1_idx_1;
  double z1_idx_2;
  b = 1.0 / sqrt(((rt_powd_snf(q[0], 2.0) + rt_powd_snf(q[1], 2.0)) +
                  rt_powd_snf(q[2], 2.0)) +
                 rt_powd_snf(q[3], 2.0));
  z1_idx_0 = q[0] * b;
  z1_idx_1 = q[1] * b;
  z1_idx_2 = q[2] * b;
  c = q[3] * b;
  unnamed_idx_0_tmp = z1_idx_1 * c * 2.0 - z1_idx_0 * z1_idx_2 * 2.0;
  if (unnamed_idx_0_tmp > 1.0) {
    unnamed_idx_0_tmp = 1.0;
  }
  if (unnamed_idx_0_tmp < -1.0) {
    unnamed_idx_0_tmp = -1.0;
  }
  if ((unnamed_idx_0_tmp < 0.0) &&
      (fabs(unnamed_idx_0_tmp + 1.0) < 2.2204460492503131E-15)) {
    a = -2.0 * b_atan2(z1_idx_1, z1_idx_0);
    c = 0.0;
    b = 1.5707963267948966;
  } else if ((unnamed_idx_0_tmp > 0.0) &&
             (fabs(unnamed_idx_0_tmp - 1.0) < 2.2204460492503131E-15)) {
    a = 2.0 * b_atan2(z1_idx_1, z1_idx_0);
    c = 0.0;
    b = -1.5707963267948966;
  } else {
    b = z1_idx_0 * z1_idx_0 * 2.0 - 1.0;
    a = b_atan2(z1_idx_0 * c * 2.0 + z1_idx_1 * z1_idx_2 * 2.0,
                b + z1_idx_1 * z1_idx_1 * 2.0);
    c = b_atan2(z1_idx_0 * z1_idx_1 * 2.0 + z1_idx_2 * c * 2.0,
                b + c * c * 2.0);
    b = -asin(unnamed_idx_0_tmp);
  }
  eul[0] = a;
  eul[1] = b;
  eul[2] = c;
}

/*
 * Arguments    : const double q[4]
 *                double R[3][3]
 * Return Type  : void
 */
static void quat2rotm(const double q[4], double R[3][3])
{
  double tempR[9];
  double b;
  double b_tempR_tmp;
  double c_tempR_tmp;
  double d_tempR_tmp;
  double e_tempR_tmp;
  double f_tempR_tmp;
  double normRowMatrix_idx_0;
  double normRowMatrix_idx_1;
  double normRowMatrix_idx_2;
  double tempR_tmp;
  int k;
  b = 1.0 / sqrt(((rt_powd_snf(q[0], 2.0) + rt_powd_snf(q[1], 2.0)) +
                  rt_powd_snf(q[2], 2.0)) +
                 rt_powd_snf(q[3], 2.0));
  normRowMatrix_idx_0 = q[0] * b;
  normRowMatrix_idx_1 = q[1] * b;
  normRowMatrix_idx_2 = q[2] * b;
  b *= q[3];
  tempR_tmp = b * b;
  b_tempR_tmp = normRowMatrix_idx_2 * normRowMatrix_idx_2;
  tempR[0] = 1.0 - 2.0 * (b_tempR_tmp + tempR_tmp);
  c_tempR_tmp = normRowMatrix_idx_1 * normRowMatrix_idx_2;
  d_tempR_tmp = normRowMatrix_idx_0 * b;
  tempR[1] = 2.0 * (c_tempR_tmp - d_tempR_tmp);
  e_tempR_tmp = normRowMatrix_idx_1 * b;
  f_tempR_tmp = normRowMatrix_idx_0 * normRowMatrix_idx_2;
  tempR[2] = 2.0 * (e_tempR_tmp + f_tempR_tmp);
  tempR[3] = 2.0 * (c_tempR_tmp + d_tempR_tmp);
  c_tempR_tmp = normRowMatrix_idx_1 * normRowMatrix_idx_1;
  tempR[4] = 1.0 - 2.0 * (c_tempR_tmp + tempR_tmp);
  tempR_tmp = normRowMatrix_idx_2 * b;
  d_tempR_tmp = normRowMatrix_idx_0 * normRowMatrix_idx_1;
  tempR[5] = 2.0 * (tempR_tmp - d_tempR_tmp);
  tempR[6] = 2.0 * (e_tempR_tmp - f_tempR_tmp);
  tempR[7] = 2.0 * (tempR_tmp + d_tempR_tmp);
  tempR[8] = 1.0 - 2.0 * (c_tempR_tmp + b_tempR_tmp);
  for (k = 0; k < 3; k++) {
    R[k][0] = tempR[3 * k];
    R[k][1] = tempR[3 * k + 1];
    R[k][2] = tempR[3 * k + 2];
  }
  for (k = 0; k < 3; k++) {
    R[0][k] = tempR[3 * k];
    R[1][k] = tempR[3 * k + 1];
    R[2][k] = tempR[3 * k + 2];
  }
}

/*
 * Arguments    : double q[4]
 * Return Type  : void
 */
static void quatinv(double q[4])
{
  double qnrm;
  qnrm = ((q[0] * q[0] + q[1] * q[1]) + q[2] * q[2]) + q[3] * q[3];
  q[0] /= qnrm;
  q[1] = -q[1] / qnrm;
  q[2] = -q[2] / qnrm;
  q[3] = -q[3] / qnrm;
}

/*
 * Arguments    : const double q[4]
 *                const double r[4]
 *                double qout[4]
 * Return Type  : void
 */
static void quatmultiply(const double q[4], const double r[4], double qout[4])
{
  qout[0] = ((q[0] * r[0] - q[1] * r[1]) - q[2] * r[2]) - q[3] * r[3];
  qout[1] = (q[0] * r[1] + r[0] * q[1]) + (q[2] * r[3] - r[2] * q[3]);
  qout[2] = (q[0] * r[2] + r[0] * q[2]) + (r[1] * q[3] - q[1] * r[3]);
  qout[3] = (q[0] * r[3] + r[0] * q[3]) + (q[1] * r[2] - r[1] * q[2]);
}

/*
 * Arguments    : double q[4]
 * Return Type  : void
 */
static void quatnormalize(double q[4])
{
  double qm;
  qm = sqrt(((rt_powd_snf(q[0], 2.0) + rt_powd_snf(q[1], 2.0)) +
             rt_powd_snf(q[2], 2.0)) +
            rt_powd_snf(q[3], 2.0));
  q[0] /= qm;
  q[1] /= qm;
  q[2] /= qm;
  q[3] /= qm;
}

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    double d;
    y = fabs(u0);
    d = fabs(u1);
    if (rtIsInf(u1)) {
      if (y == 1.0) {
        y = 1.0;
      } else if (y > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d == 0.0) {
      y = 1.0;
    } else if (d == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }
  return y;
}

/*
 * Initialize Extended Kalman Filter for combined orientation and altitude
 *  NED frame (North-East-Down), using BMP280 as barometer sensor
 *
 * Arguments    : double freq
 *                const double initState[6]
 *                snaData_t *sys
 * Return Type  : void
 */
void ekf10Init(double freq, const double initState[6], snaData_t *sys)
{
  static const double dv[6] = {1.0E-5, 1.0E-5, 1.0E-5, 1.0E-5, 0.01, 0.01};
  double b_Q_gyro_quat[6];
  double R_acc_noise[3];
  double Q_acc_base_tmp;
  double Q_gyro_quat;
  double dt;
  int i;
  dt = 1.0 / freq;
  /*  Sampling time */
  /*  State vector: [qw, qx, qy, qz, h, vh]  */
  /*  Quaternion (orientation), altitude (h), vertical velocity (vh) in NED */
  /*  Gyroscope Process Noise (MPU6500) */
  /*  MPU6500: 0.005 °/s/√Hz to rad/s/√Hz */
  /*  Continuous to discrete-time variance */
  Q_gyro_quat = 0.25 * rt_powd_snf(dt, 3.0) * 7.6154354946677164E-9;
  /*  Quaternion noise (x, y, z) */
  /*  Accelerometer Process Noise (MPU6500) */
  /*  MPU6500: 300 µg/√Hz to m/s^2/√Hz */
  Q_acc_base_tmp = 8.661249E-6 * dt;
  /*  Discrete-time variance */
  /*  Process Noise Covariance (Q) - 6x6 for quaternion and altitude/velocity */
  b_Q_gyro_quat[0] = Q_gyro_quat;
  b_Q_gyro_quat[1] = Q_gyro_quat;
  b_Q_gyro_quat[2] = Q_gyro_quat;
  b_Q_gyro_quat[3] = 7.6154354946677164E-9 * dt;
  b_Q_gyro_quat[4] = Q_acc_base_tmp;
  b_Q_gyro_quat[5] = Q_acc_base_tmp;
  diag(b_Q_gyro_quat, sys->Q);
  /*  % Quaternion */
  /*  Altitude, velocity (tuned for stability) */
  /*  Measurement Noise - Accelerometer (MPU6500) */
  /*  Discrete-time variance */
  R_acc_noise[0] = Q_acc_base_tmp;
  R_acc_noise[1] = Q_acc_base_tmp;
  R_acc_noise[2] = Q_acc_base_tmp;
  b_diag(R_acc_noise, sys->R_acc);
  /*  3x3 for accel */
  /*  Measurement Noise - Magnetometer (QMC5883P) */
  /*  QMC5883P: ~0.1 gauss noise */
  Q_gyro_quat = 0.25 * dt;
  /*  Discrete-time variance */
  R_acc_noise[0] = Q_gyro_quat;
  R_acc_noise[1] = Q_gyro_quat;
  R_acc_noise[2] = Q_gyro_quat;
  b_diag(R_acc_noise, sys->R_mag);
  /*  3x3 for mag */
  /*  Measurement Noise - Barometer (BMP280) */
  /*  BMP280 typical noise ~0.02-0.05 m, adjusted for NED */
  /*  Scalar variance for altitude */
  /*  Initial State Covariance (P) - 6x6 for combined system */
  diag(dv, sys->P);
  /*  Relaxed for convergence */
  /*  Output Combined System Structure */
  sys->euler[0] = 0.0;
  sys->euler[1] = 0.0;
  sys->euler[2] = 0.0;
  sys->dt = dt;
  sys->R_baro = 0.0025000000000000005;
  for (i = 0; i < 6; i++) {
    sys->x[i] = initState[i];
  }
  /*  % Process noise covariance */
  /*  % Accelerometer measurement noise */
  /*  % Magnetometer measurement noise */
  /*  % Barometer (BMP280) measurement noise */
  /*  % Initial state: [qw qx qy qz h vh] */
  /*  Initial covariance */
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void ekf10Init_initialize(void)
{
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void ekf10Init_terminate(void)
{
}

/*
 * Extended Kalman Filter Step for Combined Orientation and Altitude
 *  NED frame (North-East-Down), using BMP280 as barometer sensor
 *
 * Arguments    : snaData_t *sys
 *                const imuData_t *imu
 *                const double magNED[3]
 *                double alti
 * Return Type  : void
 */
void ekf10Step(snaData_t *sys, const imuData_t *imu, const double magNED[3],
               double alti)
{
  static const signed char iv[6][6] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0},
                                       {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0},
                                       {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
  double H[6][7];
  double F[6][6];
  double b_F[6][6];
  double dv[3][3];
  double a[4];
  double q_pred[4];
  double dv1[3];
  double a_idx_0_tmp;
  double a_tmp;
  double accel_norm;
  double b_a_tmp;
  double d;
  double h_pred;
  double mag_norm;
  double z_mag_idx_0;
  double z_mag_idx_1;
  double z_mag_idx_2;
  int i;
  int i1;
  int i2;
  /*  Sampling period */
  /*  Extract current state */
  /*  Quaternion [qw, qx, qy, qz] */
  /*  Altitude (positive up in NED) */
  /*  Vertical velocity (positive up in NED) */
  /*  --- Prediction Step --- */
  /*  1. Quaternion propagation (using gyroscope) */
  /*  Gyroscope quaternion (body frame rates) */
  /*  Quaternion derivative */
  /*  Propagate quaternion */
  a[0] = 0.0;
  a[1] = imu->gyro[0];
  a[2] = imu->gyro[1];
  a[3] = imu->gyro[2];
  quatmultiply(&sys->x[0], a, q_pred);
  q_pred[0] = sys->x[0] + sys->dt * (0.5 * q_pred[0]);
  q_pred[1] = sys->x[1] + sys->dt * (0.5 * q_pred[1]);
  q_pred[2] = sys->x[2] + sys->dt * (0.5 * q_pred[2]);
  q_pred[3] = sys->x[3] + sys->dt * (0.5 * q_pred[3]);
  quatnormalize(q_pred);
  /*  Normalize quaternion */
  /*  2. Transform accelerometer from body to NED frame */
  /*  Rotation matrix from body to NED */
  /*  Acceleration in NED frame */
  /*  In NED, z is down, gravity is +9.80665 m/s^2, subtract to get motion accel
   */
  quat2rotm(q_pred, dv);
  d = imu->accel[0];
  a_idx_0_tmp = imu->accel[1];
  mag_norm = imu->accel[2];
  for (i = 0; i < 3; i++) {
    dv1[i] = (dv[0][i] * d + dv[1][i] * a_idx_0_tmp) + dv[2][i] * mag_norm;
  }
  /*  Vertical acceleration (down positive) */
  /*  Convert to upward positive for altitude convention */
  /*  Motion acceleration upward (positive up) */
  /*  3. Altitude and velocity propagation (positive up) */
  h_pred = (sys->x[4] + sys->dt * sys->x[5]) +
           0.5 * (sys->dt * sys->dt) * -(dv1[2] - 9.81);
  /*  Altitude update */
  /*  Velocity update */
  /*  4. Combined predicted state */
  /*  5. State transition Jacobian (F) - Linearized process model */
  for (i = 0; i < 6; i++) {
    for (i1 = 0; i1 < 6; i1++) {
      F[i][i1] = iv[i][i1];
    }
  }
  double dv2[4][4];
  /*  6x6 identity as base */
  /*  Quaternion part (small-angle approximation) */
  mag_norm = 0.5 * sys->dt;
  b_eye(dv2);
  a_tmp = mag_norm * -imu->gyro[0];
  b_a_tmp = mag_norm * -imu->gyro[1];
  z_mag_idx_2 = mag_norm * -imu->gyro[2];
  accel_norm = mag_norm * imu->gyro[0];
  z_mag_idx_0 = mag_norm * 0.0;
  z_mag_idx_1 = mag_norm * imu->gyro[2];
  a_idx_0_tmp = mag_norm * imu->gyro[1];
  F[0][0] = dv2[0][0] + mag_norm * 0.0;
  F[0][1] = accel_norm + dv2[0][1];
  F[0][2] = a_idx_0_tmp + dv2[0][2];
  F[0][3] = z_mag_idx_1 + dv2[0][3];
  F[1][0] = dv2[1][0] + a_tmp;
  F[1][1] = z_mag_idx_0 + dv2[1][1];
  F[1][2] = z_mag_idx_2 + dv2[1][2];
  F[1][3] = a_idx_0_tmp + dv2[1][3];
  F[2][0] = dv2[2][0] + b_a_tmp;
  F[2][1] = z_mag_idx_1 + dv2[2][1];
  F[2][2] = z_mag_idx_0 + dv2[2][2];
  F[2][3] = a_tmp + dv2[2][3];
  F[3][0] = dv2[3][0] + z_mag_idx_2;
  F[3][1] = b_a_tmp + dv2[3][1];
  F[3][2] = accel_norm + dv2[3][2];
  F[3][3] = z_mag_idx_0 + dv2[3][3];
  /*  Altitude and velocity part (upward positive) */
  F[5][4] = sys->dt;
  /*  dh/dvh */
  /*  6. Predict covariance */
  for (i = 0; i < 6; i++) {
    for (i1 = 0; i1 < 6; i1++) {
      d = 0.0;
      for (i2 = 0; i2 < 6; i2++) {
        d += F[i2][i] * sys->P[i1][i2];
      }
      b_F[i1][i] = d;
    }
  }
  for (i = 0; i < 6; i++) {
    for (i1 = 0; i1 < 6; i1++) {
      d = 0.0;
      for (i2 = 0; i2 < 6; i2++) {
        d += b_F[i2][i] * F[i2][i1];
      }
      sys->P[i1][i] = d + sys->Q[i1][i];
    }
  }
  /*  --- Update Step --- */
  /*  Normalize measurements (avoid division by zero) */
  accel_norm = b_norm(imu->accel);
  if (!(accel_norm == 0.0)) {
    /*  Normalized accelerometer (body frame) */
    mag_norm = b_norm(magNED);
    if (!(mag_norm == 0.0)) {
      double c_H[7][7];
      double dv4[7][7];
      double K[7][6];
      double b_H[6][7];
      double y_tmp[7][6];
      double b_imu[7];
      double dv5[7];
      double b_q_pred[6];
      double dv3[4];
      double h_global[4];
      double b_h_global[2];
      z_mag_idx_0 = magNED[0] / mag_norm;
      z_mag_idx_1 = magNED[1] / mag_norm;
      z_mag_idx_2 = magNED[2] / mag_norm;
      /*  Normalized magnetometer (NED frame) */
      /*  Barometer measurement (BMP280, altitude up) */
      /*  7. Measurement function (h(x)) */
      /*  Accelerometer measurement model (gravity in body frame) */
      /*  Magnetometer measurement model (NED frame) */
      h_global[0] = q_pred[0];
      h_global[1] = q_pred[1];
      h_global[2] = q_pred[2];
      h_global[3] = q_pred[3];
      quatinv(h_global);
      a[0] = 0.0;
      a[1] = z_mag_idx_0;
      a[2] = z_mag_idx_1;
      a[3] = z_mag_idx_2;
      quatmultiply(a, h_global, dv3);
      quatmultiply(q_pred, dv3, h_global);
      b_h_global[0] = h_global[1];
      b_h_global[1] = h_global[2];
      d = c_norm(b_h_global);
      /*  Magnetic field components */
      /*  Barometer measurement model (altitude up) */
      /*  Combined measurement vector */
      /*  8. Measurement Jacobian (H) */
      memset(&H[0][0], 0, 42U * sizeof(double));
      /*  7 measurements (3 accel, 3 mag, 1 baro) x 6 states */
      /*  Accelerometer Jacobian (wrt quaternion only) */
      mag_norm = 2.0 * -q_pred[2];
      H[0][0] = mag_norm;
      H[1][0] = 2.0 * q_pred[3];
      H[2][0] = 2.0 * -q_pred[0];
      H[3][0] = 2.0 * q_pred[1];
      H[0][1] = 2.0 * q_pred[1];
      H[1][1] = 2.0 * q_pred[0];
      H[2][1] = 2.0 * q_pred[3];
      H[3][1] = 2.0 * q_pred[2];
      H[0][2] = 2.0 * q_pred[0];
      H[1][2] = 2.0 * -q_pred[1];
      H[2][2] = mag_norm;
      H[3][2] = 2.0 * q_pred[3];
      /*  Magnetometer Jacobian (wrt quaternion only) */
      H[0][3] = -2.0 * h_global[3] * q_pred[2];
      mag_norm = 2.0 * h_global[3] * q_pred[3];
      H[1][3] = mag_norm;
      a_tmp = 2.0 * h_global[3] * q_pred[0];
      H[2][3] = -4.0 * d * q_pred[2] - a_tmp;
      b_a_tmp = 2.0 * h_global[3] * q_pred[1];
      H[3][3] = -4.0 * d * q_pred[3] + b_a_tmp;
      H[0][4] = -2.0 * d * q_pred[3] + b_a_tmp;
      b_a_tmp = 2.0 * d * q_pred[2];
      H[1][4] = b_a_tmp + a_tmp;
      a_tmp = 2.0 * d * q_pred[1];
      H[2][4] = a_tmp + mag_norm;
      H[3][4] = -2.0 * d * q_pred[0] + 2.0 * h_global[3] * q_pred[2];
      H[0][5] = b_a_tmp;
      H[1][5] = 2.0 * d * q_pred[3] - 4.0 * h_global[3] * q_pred[1];
      H[2][5] = 2.0 * d * q_pred[0] - 4.0 * h_global[3] * q_pred[2];
      H[3][5] = a_tmp;
      /*  Barometer Jacobian (wrt altitude only) */
      H[4][6] = 1.0;
      /*  9. Combined measurement noise covariance */
      /*  10. Kalman update */
      /*  Residual */
      for (i = 0; i < 7; i++) {
        for (i1 = 0; i1 < 6; i1++) {
          y_tmp[i][i1] = H[i1][i];
        }
      }
      /*  Innovation covariance */
      blkdiag(sys->R_acc, sys->R_mag, sys->R_baro, dv4);
      for (i = 0; i < 6; i++) {
        for (i1 = 0; i1 < 7; i1++) {
          a_idx_0_tmp = 0.0;
          for (i2 = 0; i2 < 6; i2++) {
            a_idx_0_tmp += sys->P[i2][i] * y_tmp[i1][i2];
          }
          K[i1][i] = a_idx_0_tmp;
        }
      }
      for (i = 0; i < 7; i++) {
        for (i1 = 0; i1 < 6; i1++) {
          a_idx_0_tmp = 0.0;
          for (i2 = 0; i2 < 6; i2++) {
            a_idx_0_tmp += H[i2][i] * sys->P[i1][i2];
          }
          b_H[i1][i] = a_idx_0_tmp;
        }
        for (i1 = 0; i1 < 7; i1++) {
          a_idx_0_tmp = 0.0;
          for (i2 = 0; i2 < 6; i2++) {
            a_idx_0_tmp += b_H[i2][i] * y_tmp[i1][i2];
          }
          c_H[i1][i] = a_idx_0_tmp + dv4[i1][i];
        }
      }
      mrdiv(K, c_H);
      /*  Kalman gain */
      b_imu[0] = imu->accel[0] / accel_norm;
      b_imu[3] = z_mag_idx_0;
      b_imu[1] = imu->accel[1] / accel_norm;
      b_imu[4] = z_mag_idx_1;
      b_imu[2] = imu->accel[2] / accel_norm;
      b_imu[5] = z_mag_idx_2;
      b_imu[6] = alti;
      a_idx_0_tmp = q_pred[0] * q_pred[2];
      mag_norm = q_pred[1] * q_pred[3];
      a_tmp = mag_norm - a_idx_0_tmp;
      dv5[0] = 2.0 * a_tmp;
      b_a_tmp = q_pred[0] * q_pred[1] + q_pred[2] * q_pred[3];
      dv5[1] = 2.0 * b_a_tmp;
      z_mag_idx_2 = q_pred[2] * q_pred[2];
      accel_norm = q_pred[3] * q_pred[3];
      z_mag_idx_0 = q_pred[1] * q_pred[1];
      dv5[2] =
          ((q_pred[0] * q_pred[0] - z_mag_idx_0) - z_mag_idx_2) + accel_norm;
      dv5[3] = 2.0 * d * ((0.5 - z_mag_idx_2) - accel_norm) +
               2.0 * h_global[3] * a_tmp;
      dv5[4] = 2.0 * d * (q_pred[1] * q_pred[2] - q_pred[0] * q_pred[3]) +
               2.0 * h_global[3] * b_a_tmp;
      dv5[5] = 2.0 * d * (a_idx_0_tmp + mag_norm) +
               2.0 * h_global[3] * ((0.5 - z_mag_idx_0) - z_mag_idx_2);
      dv5[6] = h_pred;
      for (i = 0; i < 7; i++) {
        b_imu[i] -= dv5[i];
      }
      b_q_pred[0] = q_pred[0];
      b_q_pred[1] = q_pred[1];
      b_q_pred[2] = q_pred[2];
      b_q_pred[3] = q_pred[3];
      b_q_pred[4] = h_pred;
      b_q_pred[5] = sys->x[5] + sys->dt * -(dv1[2] - 9.81);
      /*  State update */
      eye(F);
      for (i = 0; i < 6; i++) {
        d = 0.0;
        for (i1 = 0; i1 < 7; i1++) {
          d += K[i1][i] * b_imu[i1];
        }
        sys->x[i] = b_q_pred[i] + d;
        for (i1 = 0; i1 < 6; i1++) {
          d = 0.0;
          for (i2 = 0; i2 < 7; i2++) {
            d += K[i2][i] * H[i1][i2];
          }
          b_F[i1][i] = F[i1][i] - d;
        }
        for (i1 = 0; i1 < 6; i1++) {
          d = 0.0;
          for (i2 = 0; i2 < 6; i2++) {
            d += b_F[i2][i] * sys->P[i1][i2];
          }
          F[i1][i] = d;
        }
      }
      memcpy(&sys->P[0][0], &F[0][0], 36U * sizeof(double));
      /*  Covariance update */
      /*  11. Normalize quaternion in updated state */
      h_global[0] = sys->x[0];
      h_global[1] = sys->x[1];
      h_global[2] = sys->x[2];
      h_global[3] = sys->x[3];
      quatnormalize(h_global);
      sys->x[0] = h_global[0];
      sys->x[1] = h_global[1];
      sys->x[2] = h_global[2];
      sys->x[3] = h_global[3];
      /*  12. Compute Euler angles for output (NED convention) */
      quat2eul(&sys->x[0], sys->euler);
      /*  [yaw, pitch, roll] in radians */
    }
  }
}

/*
 * Arguments    : const double meanAccel[3]
 *                const double meanMag[3]
 *                double meanAlti
 *                double initState[6]
 * Return Type  : void
 */
void initStateEKF(const double meanAccel[3], const double meanMag[3],
                  double meanAlti, double initState[6])
{
  double q_b;
  double q_c;
  double q_d;
  int i;
  for (i = 0; i < 6; i++) {
    initState[i] = 0.0;
  }
  double q_a;
  q_a = ecompass(meanAccel, meanMag, &q_b, &q_c, &q_d);
  initState[0] = q_a;
  initState[1] = q_b;
  initState[2] = q_c;
  initState[3] = q_d;
  initState[4] = meanAlti;
}

/*
 * File trailer for ekf10Init.c
 *
 * [EOF]
 */
