#include "CscMatrix.h"
#include <string>

int main(int argc, char *argv[]) {
    std::string filename(argv[1]);

    CscMatrix cscMatrix;

    int n, nz, *rows, *colPtrs;
    double* values;
    cscMatrix.loadCscMatrix(filename, n, nz, rows, colPtrs, values);
    cscMatrix.printCscMatrix(n, nz, rows, colPtrs, values);
}

