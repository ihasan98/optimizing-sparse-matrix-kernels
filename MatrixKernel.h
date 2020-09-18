#ifndef OPTIMIZING_SPARSE_MATRIX_KERNELS_MATRIXKERNEL_H
#define OPTIMIZING_SPARSE_MATRIX_KERNELS_MATRIXKERNEL_H

/*
 * Lower triangular solver Lx=b
 * L is stored in the compressed column storage format
 * Inputs are:
 * n : the matrix dimension
 * Lp : the column pointer of L
 * Li : the row index of L
 * Lx : the values of L
 * In/Out:
 * x : the right hand-side b at start and the solution x at the end.
 */
int lsolveBasic(int n, int *Lp, int *Li, double *Lx, double *x);

int lsolveOptimized(int n, int *Lp, int *Li, double *Lx, double *x);

int lsolveParallel(int n, int *Lp, int *Li, double *Lx, double *x,
                   int numLevels, int *ilev, int *jlev);

/*
 *This returns the number of levels. [jlev] is an array that lists the unknowns
 *in a non decreasing order of their levels. [ilev] contains the pointers to the
 *levels in [jlev]
 */
int buildLevelSets(int n, int nz, int *Lp, int *Li, int *&ilev, int *&jlev);

#endif // OPTIMIZING_SPARSE_MATRIX_KERNELS_MATRIXKERNEL_H
