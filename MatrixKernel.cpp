#include "MatrixKernel.h"

int MatrixKernel::lsolve_basic(int n, int *Lp, int *Li, double *Lx, double *x) {
    int p, j;
    if (!Lp || !Li || !x) return (0); /* check inputs */
    for (j = 0; j < n; j++) {
        x[j] /= Lx[Lp[j]];
        for (p = Lp[j] + 1; p < Lp[j + 1]; p++) {
            x[Li[p]] -= Lx[p] * x[j];
        }
    }
    return (1);
}