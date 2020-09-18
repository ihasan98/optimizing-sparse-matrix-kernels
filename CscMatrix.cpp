#include "CscMatrix.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool loadCscMatrix(std::string &filename, int &m, int &n, int &nz, int *&rows,
                   int *&colPtrs, double *&values) {
  std::ifstream matrixMarket(filename, std::ios::in);
  if (!matrixMarket.is_open()) {
    std::cout << "Unable to open MatrixMarket file." << std::endl;
    return false;
  }

  std::string line, format;
  std::getline(matrixMarket, line);

  if (!validateHeaderLine(line, format)) {
    // TODO: Improve error messages
    std::cout << "This matrix is not in an appropriate format!" << std::endl;
    return false;
  };

  // Skipping comments
  while (line[0] == '%') {
    std::getline(matrixMarket, line);
  }

  if (!getDims(line, format, m, n, nz)) {
    std::cout << "There is a problem reading matrix dimensions." << std::endl;
  }

  assert(format == "coordinate");

  // TODO: Refactor this to perhaps use C++ smart pointers.
  values = (double *)malloc(sizeof(double) * nz);
  rows = (int *)malloc(sizeof(int) * nz);
  colPtrs = (int *)malloc(sizeof(int) * (n + 1));

  if (!values || !rows || !colPtrs) {
    std::cout << "Error init-ing memory" << std::endl;
    return false;
  }

  coordinateFormatToCsc(matrixMarket, n, nz, rows, colPtrs, values);

  return true;
}

bool loadRhsVector(std::string &filename, int &n, double *&values) {
  std::ifstream matrixMarket(filename, std::ios::in);
  if (!matrixMarket.is_open()) {
    std::cout << "Unable to open MatrixMarket file." << std::endl;
    return false;
  }

  std::string line, format;
  std::getline(matrixMarket, line);

  if (!validateHeaderLine(line, format)) {
    // TODO: Improve error messages
    std::cout << "This matrix is not in an appropriate format!" << std::endl;
    return false;
  };

  // Skipping comments
  while (line[0] == '%') {
    std::getline(matrixMarket, line);
  }

  int cols, nz;
  if (!getDims(line, format, n, cols, nz)) {
    std::cout << "There is a problem reading matrix dimensions." << std::endl;
  }

  assert(cols == 1);
  values = (double *)malloc(sizeof(double) * n * 1);

  if (!values) {
    std::cout << "Error init-ing memory" << std::endl;
    return false;
  }

  memset(values, 0, sizeof(double) * n * 1);

  int row, col;
  double value;
  if (format == "coordinate") {
    for (int i = 0; i < nz; ++i) {
      matrixMarket >> row >> col >> value;
      --row;

      values[row] = value;
    }
  } else if (format == "array") {
    arrayFormatToVals(matrixMarket, n, 1, values);
  }

  return true;
}

void printCscMatrix(const int &n, const int &nz, int *&rows, int *&colPtrs,
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

void coordinateFormatToCsc(std::ifstream &matrixMarket, const int &n,
                           const int &nz, int *&rows, int *&colPtrs,
                           double *&values) {
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
}

void arrayFormatToVals(std::ifstream &matrixMarket, const int &m, const int &n,
                       double *&values) {
  for (int i = 0; i < m * n; ++i) {
    matrixMarket >> values[i];
  }
}

bool validateHeaderLine(const std::string &line, std::string &format) {
  std::istringstream iss(line);
  std::string banner, object, field, symmetry;

  if (!(iss >> banner >> object >> format >> field >> symmetry)) {
    return false;
  }

  return (banner == "%%MatrixMarket" || object == "matrix" || field == "real");
}

bool getDims(const std::string &line, const std::string &format, int &m, int &n,
             int &nz) {
  std::istringstream iss(line);

  if (format == "coordinate") {
    return !(!(iss >> m >> n >> nz));
  } else if (format == "array") {
    return !(!(iss >> m >> n));
  }

  return false;
}
