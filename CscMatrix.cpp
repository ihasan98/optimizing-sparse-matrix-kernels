#include "CscMatrix.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

bool CscMatrix::loadCscMatrix(std::string filename, int &n, int &nz, int *&rows, int *&colPtrs, double *&values) {
    std::ifstream matrixMarket(filename, std::ios::in);
    if (!matrixMarket.is_open()) {
        std::cout << "Unable to open MatrixMarket file." << std::endl;
        return false;
    }

    std::string line;
    std::getline(matrixMarket, line);

    if (!validateHeaderLine(line)) {
        //TODO: Improve error messages
        std::cout << "This matrix is not in an appropriate format!" << std::endl;
        return false;
    };

    // Skipping comments
    while (line[0] == '%') {
        std::getline(matrixMarket, line);
    }

    if (!getDims(line, n, nz)) {
        std::cout << "There is a problem reading matrix dimensions." << std::endl;
    }

    //TODO: Refactor this to perhaps use C++ smart pointers.
    values = (double *) malloc(sizeof(double) * nz);
    rows = (int *) malloc(sizeof(int) * nz);
    colPtrs = (int *) malloc(sizeof(int) * (n + 1));

    if (!values || !rows || !colPtrs) {
        std::cout << "Error init-ing memory" << std::endl;
        return false;
    }

    int row, col, curCol, elementsInCurCol;
    double value;

    curCol = 0;
    elementsInCurCol = 0;

    for (int i = 0; i < nz; ++i) {
        matrixMarket >> row >> col >> value;
        --row;
        --col; // Ensure zero indexing

        if (col != curCol) {
            colPtrs[curCol + 1] = colPtrs[curCol] + elementsInCurCol;
            while (++curCol != col) {
                colPtrs[curCol + 1] = colPtrs[curCol];
            }
            elementsInCurCol = 0;
        }

        values[i] = value;
        rows[i] = row;
        ++elementsInCurCol;
    }

    colPtrs[n] = colPtrs[n - 1] + elementsInCurCol;

    return true;
}

void CscMatrix::printCscMatrix(const int &n, const int &nz, int *&rows, int *&colPtrs,
                               double *&values) {
    std::cout << "COLS:" << std::endl;

    for (int i = 0; i <= n; ++i) {
        std::cout << colPtrs[i] << " ";
    }

    std::cout << std::endl << "ROWS:" << std::endl;

    for (int i = 0; i < nz; ++i) {
        std::cout << rows[i] << " ";
    }

    std::cout << std::endl << "VALUES:" << std::endl;

    for (int i = 0; i < nz; ++i) {
        std::cout << values[i] << " ";
    }


}

bool CscMatrix::validateHeaderLine(const std::string &line) {
    std::istringstream iss(line);
    std::string banner, object, format, field, symmetry;

    if (!(iss >> banner >> object >> format >> field >> symmetry)) {
        return false;
    }

    return (banner == "%%MatrixMarket" || object == "matrix" || format == "coordinate" || field == "real");
}

bool CscMatrix::getDims(const std::string &line, int &n, int &nz) {
    std::istringstream iss(line);

    return !(!(iss >> n >> n >> nz));
}


