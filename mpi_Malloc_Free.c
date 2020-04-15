#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define global_N 12

void print(int M, int N, float A[M][N])
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
   float global_A[global_N][global_N], *local_A;
   int local_N, i, j, k, l, m, n;
   int np2, np, pid;
   MPI_Status status;
   int tag, dest=0;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np2);

   np = sqrt(np2);
   local_N = global_N/np;
	k = i/np; l = i%np;

   local_A = (float*)malloc(sizeof(float)*local_N*local_N);

   if (pid == 0) {
      // initialization
      for (i=0; i<global_N; i++) {
         for (j=0; j<global_N; j++) {
            global_A[i][j] = (i*global_N+j)*0.001;
         }
      }
      print(global_N, global_N, global_A);
      for(i=1; i<np2; i++) {
         for(j=0;j<local_N;j++){
				k = i/np; l = i%np;
            MPI_Send(&global_A[k*local_N+j][l*local_N], local_N, MPI_FLOAT, i, tag, MPI_COMM_WORLD);
         }
      }
	}
   else{
      for(j=0;j<local_N;j++){
         MPI_Recv(&local_A[j*local_N], local_N, MPI_FLOAT, dest, tag, MPI_COMM_WORLD, &status);
      }
   }

	// add pid
   for (i=0; i<local_N*local_N; i++)
      local_A[i] += pid;

	if(pid!=0){
		for(j=0;j<local_N;j++){
			MPI_Send(&local_A[j*local_N], local_N, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
		}
	}
	else {
		for(i=1;i<np2;i++){
			for(j=0;j<local_N;j++){
				k = i/np; l = i%np;
				MPI_Recv(&global_A[k*local_N+j][l*local_N], local_N, MPI_FLOAT, i, tag, MPI_COMM_WORLD, &status);
			}
		}
      printf("\n");
      print(global_N, global_N, global_A);
   }

   free(local_A);
   MPI_Finalize();
	exit(0);
}
