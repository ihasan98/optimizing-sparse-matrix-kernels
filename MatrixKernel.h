#ifndef OPTIMIZING_SPARSE_MATRIX_KERNELS_MATRIXKERNEL_H
#define OPTIMIZING_SPARSE_MATRIX_KERNELS_MATRIXKERNEL_H
#define EPSILON 1E6

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

/*
 * Sparse matrix-vector multiply: y = A*x
 * A is stored in the compressed column storage format
 * Inputs:
 * Ap : the column pointer of A
 * Ai : the row index of A
 * Ax : the values of A
 * x : is a dense vector
 * Output:
 * y : is a dense vector that stores the result of multiplication
 */
int spmvCsc(int n, int *Ap, int *Ai, double *Ax, double *x, double *y);

/*
 * Ensures that Lx = b
 */
void validateTriangularSolve(int n, int *Lp, int *Li, double *Lx, double *b,
                             double *x);
#endif // OPTIMIZING_SPARSE_MATRIX_KERNELS_MATRIXKERNEL_H
