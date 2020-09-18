#include "CscMatrix.h"
#include "MatrixKernel.h"
#include <cassert>
#include <chrono>
#include <cstring>
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
    printf("-v validates the results before printing timing stats \n");
    printf("-mode serial_basic | serial_optimized | parallel \n");
    return 0;
  }

  int validate = find_option(argc, argv, "-v");

  std::string fileForL(read_string(argc, argv, "-L", (char *)""));
  std::string fileForB(read_string(argc, argv, "-b", (char *)""));

  int L_m, L_n, L_nz, *Li, *Lp;
  double *Lx;
  loadCscMatrix(fileForL, L_m, L_n, L_nz, Li, Lp, Lx);

  int b_n;
  double *b;
  loadRhsVector(fileForB, b_n, b);

  assert(L_n == b_n);

  double x[b_n];
  memcpy(x, b, sizeof(double) * b_n); // Making x = b

  std::chrono::time_point<std::chrono::system_clock> start, end;
  std::chrono::duration<double> elapsed_time;

  std::string mode(read_string(argc, argv, "-mode", (char *)""));

  if (mode == "serial_basic") {
    start = std::chrono::system_clock::now();
    lsolveBasic(L_n, Lp, Li, Lx, x);
    end = std::chrono::system_clock::now();
  } else if (mode == "serial_optimized") {
    start = std::chrono::system_clock::now();
    lsolveOptimized(L_n, Lp, Li, Lx, x);
    end = std::chrono::system_clock::now();
  } else if (mode == "parallel") {
    int *ilev, *jlev;
    int numLevels = buildLevelSets(L_n, L_nz, Lp, Li, ilev, jlev);
    start = std::chrono::system_clock::now();
    lsolveParallel(L_n, Lp, Li, Lx, x, numLevels, ilev, jlev);
    end = std::chrono::system_clock::now();
  } else {
    std::cout << "Incorrect mode" << std::endl;
    exit(1);
  }
  elapsed_time = end - start;

  if (validate >= 0) {
    validateTriangularSolve(L_n, Lp, Li, Lx, b, x);
    std::cout << "Final output has been verified to be correct." << std::endl;
  }

  std::cout << mode << " " << elapsed_time.count() * 1000 << "ms" << std::endl;
}
