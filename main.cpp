#include "CscMatrix.h"
#include "MatrixKernel.h"
#include "string.h"
#include <cassert>
#include <chrono>
#include <iostream>
#include <string>

//
//  command line option processing
//
int find_option(int argc, char **argv, const char *option) {
  for (int i = 1; i < argc; i++)
    if (strcmp(argv[i], option) == 0)
      return i;
  return -1;
}

char *read_string(int argc, char **argv, const char *option,
                  char *default_value) {
  int iplace = find_option(argc, argv, option);
  if (iplace >= 0 && iplace < argc - 1)
    return argv[iplace + 1];
  return default_value;
}

int main(int argc, char *argv[]) {

  if (find_option(argc, argv, "-h") >= 0) {
    printf("Options:\n");
    printf("-h to see this help\n");
    printf("-L <filename> file where matrix L is stored \n");
    printf("-b <filename> file where vector b is stored \n");
    printf("-mode serial_basic | serial_optimized | parallel \n");
    return 0;
  }

  std::string fileForL(read_string(argc, argv, "-L", (char *)""));
  std::string fileForB(read_string(argc, argv, "-b", (char *)""));

  CscMatrix cscMatrix;

  int L_m, L_n, L_nz, *Li, *Lp;
  double *Lx;
  cscMatrix.loadCscMatrix(fileForL, L_m, L_n, L_nz, Li, Lp, Lx);

  int b_n;
  double *bx;
  cscMatrix.loadRhsVector(fileForB, b_n, bx);

  assert(L_n == b_n);

  std::chrono::time_point<std::chrono::system_clock> start, end;
  std::chrono::duration<double> elapsed_time;

  std::string mode(read_string(argc, argv, "-mode", (char *)""));

  if (mode == "serial_basic") {
    start = std::chrono::system_clock::now();
    lsolve_basic(L_n, Lp, Li, Lx, bx);
    end = std::chrono::system_clock::now();
    elapsed_time = end - start;
    std::cout << "Basic Serial: " << elapsed_time.count() << std::endl;
  } else if (mode == "serial_optimized") {
    start = std::chrono::system_clock::now();
    lsolve_optimized(L_n, Lp, Li, Lx, bx);
    end = std::chrono::system_clock::now();
    elapsed_time = end - start;
    std::cout << "Optimized Serial: " << elapsed_time.count() << std::endl;
  }
}
