#ifndef OPTIMIZING_SPARSE_MATRIX_KERNELS_CSCMATRIX_H
#define OPTIMIZING_SPARSE_MATRIX_KERNELS_CSCMATRIX_H

#include <string>

class CscMatrix {
public:
    /*
     * Initializes a CSC formatted matrix using the matrix market format stored at [filename]
     */
    bool loadCscMatrix(std::string filename, int &n, int &nz, int *&rows, int *&colPtrs, double *&values);

    void printCscMatrix(const int &n, const int &nz, int *&rows, int *&colPtrs, double *&values);

private:
    bool validateHeaderLine(const std::string &line);

    bool getDims(const std::string &line, int &n, int &nz);
};


#endif //OPTIMIZING_SPARSE_MATRIX_KERNELS_CSCMATRIX_H
