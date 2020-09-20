#include "MatrixKernel.h"
#include "string.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

int lsolveBasic(int n, int *Lp, int *Li, double *Lx, double *x) {
  int p, j;
  if (!Lp || !Li || !x)
    return (0); /* check inputs */
  for (j = 0; j < n; j++) {
    x[j] /= Lx[Lp[j]];
    for (p = Lp[j] + 1; p < Lp[j + 1]; p++) {
      x[Li[p]] -= Lx[p] * x[j];
    }
  }
  return (1);
}

int lsolveOptimized(int n, int *Lp, int *Li, double *Lx, double *x) {
  int p, j;
  if (!Lp || !Li || !x)
    return (0); /* check inputs */
  for (j = 0; j < n; j++) {
    if (x[j] != 0) {
      x[j] /= Lx[Lp[j]];
      const double x_j = x[j];
      for (p = Lp[j] + 1; p < Lp[j + 1]; p++) {
        x[Li[p]] -= Lx[p] * x_j;
      }
    }
  }
  return (1);
}

int lsolveParallel(int n, int *Lp, int *Li, double *Lx, double *x,
                   int numLevels, int *ilev, int *jlev) {
  int m, j, p;
  if (!Lp || !Li || !x)
    return (0); /* check inputs */

  for (m = 0; m < numLevels; ++m) {
#pragma omp parallel for default(shared) schedule(dynamic, 4)
    for (int k = ilev[m]; k < ilev[m + 1]; ++k) {
      j = jlev[k];
      x[j] /= Lx[Lp[j]];

      for (p = Lp[j] + 1; p < Lp[j + 1]; ++p) {
        double tmp = Lx[p] * x[j];
        int index = Li[p];
#pragma omp atomic
        x[index] -= tmp;
      }
    }
  }

  return (1);
}

// TODO: Comment/document this code
int buildLevelSets(int n, int nz, int *Lp, int *Li, int *&ilev, int *&jlev) {
  int levels[n];
  int countAtEachLevel[n];
  memset(levels, 0, sizeof(int) * n);
  memset(countAtEachLevel, 0, sizeof(int) * n);
  countAtEachLevel[0] = n;

  int maxLevel = 0;
  int row;

  for (int j = 0; j < n; ++j) {
    for (int i = Lp[j]; i < Lp[j + 1]; ++i) {
      row = Li[i];
      (countAtEachLevel[levels[row]])--;
      levels[row] = std::max(levels[j] + 1, levels[row]);
      (countAtEachLevel[levels[row]])++;
      maxLevel = std::max(maxLevel, levels[row]);
    }
  }

  int numLevels = maxLevel + 1;
  int eachLevelPointer[numLevels];
  memset(eachLevelPointer, 0, sizeof(int) * numLevels);

  ilev = (int *)malloc(sizeof(int) * (numLevels + 1));
  ilev[0] = 0;

  for (int i = 1; i < numLevels + 1; ++i) {
    ilev[i] = ilev[i - 1] + countAtEachLevel[i - 1];
  }

  assert(ilev[numLevels] == n);

  jlev = (int *)malloc(sizeof(int) * n);

  for (int i = 0; i < n; ++i) {
    int level = levels[i];
    jlev[ilev[level] + eachLevelPointer[level]] = i;
    eachLevelPointer[level]++;
  }

  return numLevels;
}

int spmvCsc(int n, int *Ap, int *Ai, double *Ax, double *x, double *y) {
  int p, j;
  if (!Ap || !x || !y)
    return (0);
  for (j = 0; j < n; j++) {
    for (p = Ap[j]; p < Ap[j + 1]; p++) {
      y[Ai[p]] += Ax[p] * x[j];
    }
  }
  return (1);
}

bool areDoubleSame(double a, double b) {
  return (fabs(a - b) <= EPSILON * std::max({1.0, fabs(a), fabs(b)}));
}

void validateTriangularSolve(int n, int *Lp, int *Li, double *Lx, double *b,
                             double *x) {
  double y[n];
  memset(y, 0.0, sizeof(double) * n);

  spmvCsc(n, Lp, Li, Lx, x, y);

  for (int i = 0; i < n; ++i) {
    assert(areDoubleSame(y[i], b[i]));
  }
}
