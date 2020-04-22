#include <stdio.h>

#define N 12

#define THREADS_X 3
#define THREADS_Y 4

#define A(i,j) A[i*N+j]
#define B(i,j) B[i*N+j]
#define C(i,j) C[i*N+j]

__global__ void index(int *A, int *B, int *C)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	int j = blockDim.y * blockIdx.y + threadIdx.y;

	C(i,j) = A(i,j) + B(i,j);
}

int main()
{
   int A[N*N], B[N*N], C[N*N], *A_d, *B_d, *C_d;
   int i, j;

	dim3 dimBlock(THREADS_X, THREADS_Y);
	dim3 dimGrid(N/THREADS_X, N/THREADS_Y);

	cudaMalloc((void **)&A_d, sizeof(int)*N*N);
	cudaMalloc((void **)&B_d, sizeof(int)*N*N);
	cudaMalloc((void **)&C_d, sizeof(int)*N*N);

	// 초기화
   for (i=0; i<N; i++){
		for(j=0; j<N; j++){
			A(i,j) = i+j;
	      B(i,j) = i*j;
		}
	}

	cudaMemcpy(A_d, A, sizeof(int)*N*N, cudaMemcpyHostToDevice);
	cudaMemcpy(B_d, B, sizeof(int)*N*N, cudaMemcpyHostToDevice);

	index <<< dimGrid, dimBlock >>> (A_d, B_d, C_d);

	cudaMemcpy(C, C_d, sizeof(int)*N*N, cudaMemcpyDeviceToHost);

	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			printf("%d ", C(i,j));
		}
		printf("\n");
	}

	cudaFree(A_d);
	cudaFree(B_d);
	cudaFree(C_d);
}
