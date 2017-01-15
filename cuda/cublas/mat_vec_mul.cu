#include <cstdio>
#include <cstdlib>
#include <cuda.h>
#include "cublas_v2.h"
#include "../common/common.h"

const int M = 1024; // # of rows
const int N = 1024; // # of columns

void generate_random_dense_matrix(int M, int N, float** A) {
  float* ret = (float*) malloc(sizeof(float) * M * N);
  // Considering locality of space, you should put N as the first for loop round
  for (int j = 0; j < N; ++j) { // A should be column-based
    for (int i = 0; i < M; ++i) {
      ret[j * N + i] = (double)rand() / RAND_MAX * 100;
    }
  }
  *A = ret;
}

void generate_random_vector(int N, float** A) {
  float* ret = (float*)malloc(sizeof(float) * N);
  for (int i = 0; i < N; ++i) {
    ret[i] = (double)rand() / RAND_MAX * 100;
  }
  *A = ret;
}

int main() {
  float *A, *dA; // source matrix in host memroy, device memory
  float *X, *dX;
  float *Y, *dY;
  float alpha = 3.0f;
  float beta = 4.0f;
  cublasHandle_t  handle = 0;

  srand(9384);
  generate_random_dense_matrix(M, N, &A);
  generate_random_vector(N, &X);
  generate_random_vector(M, &Y);

  CHECK_CUBLAS(cublasCreate(&handle));

  CHECK(cudaMalloc((void **)&dA, sizeof(float) * N * M));
  CHECK(cudaMalloc((void **)&dX, sizeof(float) * N));
  CHECK(cudaMalloc((void **)&dY, sizeof(float) * M));

  CHECK_CUBLAS(cublasSetVector(N, sizeof(float),  X, 1, dX, 1));
  CHECK_CUBLAS(cublasSetVector(M, sizeof(float),  Y, 1, dY, 1));
  CHECK_CUBLAS(cublasSetMatrix(M, N, sizeof(float),  A, M, dA, M));

  CHECK_CUBLAS(cublasSgemv(handle, CUBLAS_OP_N, M, N, &alpha, dA, M, dX, 1,
                           &beta, dY, 1));

  CHECK_CUBLAS(cublasGetVector(M, sizeof(float), dY, 1, Y, 1));
  for (int i = 0; i < 10; ++i) {
    printf("%2.2f\n", Y[i]);
  }

  free(A);
  free(X);
  free(Y);
  CHECK(cudaFree(dA));
  CHECK(cudaFree(dX));
  CHECK(cudaFree(dY));
  CHECK_CUBLAS(cublasDestroy(handle));

  return 0;
}
