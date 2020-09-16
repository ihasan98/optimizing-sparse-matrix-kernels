CC = g++
OPENMP = -fopenmp
CFLAGS = -O3

TARGETS = main

all:	$(TARGETS)

main: main.o CscMatrix.o MatrixKernel.o
	$(CC) -o $@ $(LIBS) main.o CscMatrix.o MatrixKernel.o

main.o: main.cpp
	$(CC) -c $(CFLAGS) main.cpp
CscMatrix.o: CscMatrix.cpp CscMatrix.h
	$(CC) -c $(CFLAGS) CscMatrix.cpp
MatrixKernel.o: MatrixKernel.cpp MatrixKernel.h
	$(CC) -c $(CFLAGS) MatrixKernel.cpp
clean:
	rm -f *.o $(TARGETS) *.stdout *.txt
