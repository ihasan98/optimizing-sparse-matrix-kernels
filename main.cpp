#include "CscMatrix.h"
#include "MatrixKernel.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
    std::string fileForL(argv[1]);
    std::string fileForB(argv[2]);

    CscMatrix cscMatrix;
    MatrixKernel matrixKernel;

    int L_m, L_n, L_nz, *Li, *Lp;
    double* Lx;
    cscMatrix.loadCscMatrix(fileForL, L_m, L_n, L_nz, Li, Lp, Lx);

    int b_m, b_n, b_nz, *bi, *bp;
    double* bx;
    cscMatrix.loadCscMatrix(fileForB, b_m, b_n, b_nz, bi, bp, bx);

    assert(b_n == 1);
    assert(L_n == b_m);

    matrixKernel.lsolve_basic(L_n, Lp, Li, Lx, bx);

    for (int i = 0; i < b_m; ++i) {
        std::cout << bx[i] << std::endl;
    }
}

