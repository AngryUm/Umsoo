#include<stdio.h>

#define N 160
#define THREADS 16

__global__ void sum(float *A, float *B, float *result_d)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	result_d[i] = A[i] * B[i];

	__shared__ float sresult[THREADS];

	sresult[threadIdx.x] = result_d[i];

	for(unsigned int s = blockDim.x/2; s>0; s>>=1)
	{
		if(threadIdx.x < s){
			sresult[threadIdx.x] += sresult[threadIdx.x+s];
			__syncthreads();
		}
	}
	if(threadIdx.x == 0){
		result_d[blockIdx.x] = sresult[threadIdx.x];
	}
}

int main()
{
   float A[N], B[N], tresult, *result;
	float *A_d, *B_d, *result_d;
	int i;
	
	dim3 dimBlock(THREADS);
	dim3 dimGrid((N+dimBlock.x-1)/dimBlock.x);

	for(i=0; i<N; i++){
		A[i] = i * 2;
		B[i] = N - i;
	}

	cudaMalloc((void **) &A_d, sizeof(float)*N);
	cudaMalloc((void **) &B_d, sizeof(float)*N);
	cudaMalloc((void **) &result_d, sizeof(float)*dimGrid.x);

	cudaMemcpy(A_d, A, sizeof(float)*N, cudaMemcpyHostToDevice);
	cudaMemcpy(B_d, B, sizeof(float)*N, cudaMemcpyHostToDevice);

	sum<<<dimGrid, dimBlock>>>(A_d, B_d, result_d);

	result = (float*)malloc(sizeof(float)*dimGrid.x);
	cudaMemcpy(result, result_d, sizeof(float)*dimGrid.x, cudaMemcpyDeviceToHost);

	tresult = 0.0;
	for(i=0; i<dimGrid.x; i++){
		tresult += result[i];
	}

	printf("GPU dotprod : %f\n", tresult);

	cudaFree(A_d);
	cudaFree(B_d);
	cudaFree(result_d);
	free(result);
}

