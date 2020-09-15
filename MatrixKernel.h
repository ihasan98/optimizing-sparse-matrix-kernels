#ifndef OPTIMIZING_SPARSE_MATRIX_KERNELS_MATRIXKERNEL_H
#define OPTIMIZING_SPARSE_MATRIX_KERNELS_MATRIXKERNEL_H


class MatrixKernel {
public:
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
    int lsolve_basic(int n, int *Lp, int *Li, double *Lx, double *x);

};


#endif //OPTIMIZING_SPARSE_MATRIX_KERNELS_MATRIXKERNEL_H
