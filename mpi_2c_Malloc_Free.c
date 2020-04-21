#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define global_N 12

float **malloc_2d(int row, int col)
{
   float **A, *ptr;
   int len, i;

   len = sizeof(float *)*row + sizeof(float)*col*row;
   A = (float **)malloc(len);
   ptr = (float *)(A + row);
   for(i = 0; i < row; i++)
      A[i] = (ptr + col*i);
   return A;
}

void print(int M, int N, float **A)
{
   int i, j, k;

   for (i=0; i<M; i++) {
      for (j=0; j<N; j++)
         printf("%5.3f ", A[i][j]);
      printf("\n");
   }
}

main(int argc, char* argv[])
{
   float **global_A, **global_B, **global_C;
   float **local_A, **local_B, **local_C;
   int local_N, *displs, *counts, i, j, n;
   int np2, np, pid, temp;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np2);

   np = sqrt(np2);
   local_N = global_N/np;
	n = ((global_N*global_N)/np2);

   global_A = malloc_2d(global_N, global_N);
   global_B = malloc_2d(global_N, global_N);
   global_C = malloc_2d(global_N, global_N);

   local_A = malloc_2d(local_N, local_N);
   local_B = malloc_2d(local_N, local_N);
   local_C = malloc_2d(local_N, local_N);

   // initializaton global_A and global_B
   if (pid == 0) {
      for (i=0; i<global_N; i++)
         for (j=0; j<global_N; j++) {
            global_B[i][j] = (global_N*global_N-i*global_N-j)*0.001;
            global_A[i][j] = (i*global_N+j)*0.001;
         }

		printf("global A[][] : \n");
		print(global_N, global_N, global_A);
		printf("\n");
		printf("global B[][] : \n");
		print(global_N, global_N, global_B);
   }

	counts = (int*)malloc(sizeof(int)*np2);
	displs = (int*)malloc(sizeof(int)*np2);

	for(i=0; i<np2; i++){
		counts[i] = local_N;
		if(i%np == 0){
			temp = i*local_N*local_N;
		}
		else{
			temp += local_N;	
		}
		displs[i] = temp;
	}

   // decompose global_A and global_B into local_A and local_B
		for(i=0; i<local_N; i++){
			MPI_Scatterv(&(global_A[i][0]), counts, displs, MPI_FLOAT, &(local_A[i][0]), local_N, MPI_FLOAT, 0, MPI_COMM_WORLD);
			MPI_Scatterv(&(global_B[i][0]), counts, displs, MPI_FLOAT, &(local_B[i][0]), local_N, MPI_FLOAT, 0, MPI_COMM_WORLD);
		}
	
   // local_C = local_A + local_B
   for (i=0; i<local_N; i++)
      for (j=0; j<local_N; j++)
         local_C[i][j] = local_A[i][j] + local_B[i][j];
   
   // compose local_C to global_C
	for(i=0; i<local_N; i++){
		MPI_Gatherv(&(local_C[i][0]), local_N, MPI_FLOAT, &(global_C[i][0]), counts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);
	}
   // print the result
   if (pid == 0) {
      printf("\n");
		printf("global C[][] : \n");
      print(global_N, global_N, global_C);
   }

   free(global_A);
   free(global_B);
   free(global_C);
   free(local_A);
   free(local_B);
   free(local_C);
   free(displs);
   free(counts);

   MPI_Finalize();
}
