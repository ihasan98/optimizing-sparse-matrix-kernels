#ifndef OPTIMIZING_SPARSE_MATRIX_KERNELS_CSCMATRIX_H
#define OPTIMIZING_SPARSE_MATRIX_KERNELS_CSCMATRIX_H

#include <string>

/*
 * Initializes a CSC formatted matrix using the matrix market format stored at
 * [filename]
 */
bool loadCscMatrix(std::string &filename, int &m, int &n, int &nz, int *&rows,
                   int *&colPtrs, double *&values);

bool loadRhsVector(std::string &filename, int &n, double *&values);

void printCscMatrix(const int &n, const int &nz, int *&rows, int *&colPtrs,
                    double *&values);

bool validateHeaderLine(const std::string &line, std::string &format);

bool getDims(const std::string &line, const std::string &format, int &m, int &n,
             int &nz);

void coordinateFormatToCsc(std::ifstream &matrixMarket, const int &n,
                           const int &nz, int *&rows, int *&colPtrs,
                           double *&values);

void arrayFormatToVals(std::ifstream &matrixMarket, const int &m, const int &n,
                       double *&values);

#endif // OPTIMIZING_SPARSE_MATRIX_KERNELS_CSCMATRIX_H
