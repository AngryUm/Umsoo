#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

main(int argc, char *argv[])
{
   int N = 24, i, j;
   int A[N],B[N],C[N];
   int np, pid, dest = 0, source = 0, tag = 0;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   if(pid == 0){
      for (i=0; i<N; i++) {
         A[i] = i+10;
         B[i] = i+20;
      }

      for(i=0;i<N/np;i++)
         C[i]=A[i]+B[i];

      for(i=1;i<np;i++){
         MPI_Send(A+(i*N/np), N/np, MPI_INT, i, tag, MPI_COMM_WORLD);
         MPI_Send(B+(i*N/np), N/np, MPI_INT, i, tag, MPI_COMM_WORLD);
      }
      for(i=1;i<np;i++){
         MPI_Recv(C+(i*N/np), N/np, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
      }
      
      for(i=0;i<N;i++)
         printf("%d ", C[i]);
      printf("\n");
   }
   else{
      MPI_Recv(A, N/np, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(B, N/np, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
      for(i=0;i<N/np;i++){
         C[i]=A[i]+B[i];
      }
      MPI_Send(C, N/np, MPI_INT, source, tag, MPI_COMM_WORLD);
   }

   MPI_Finalize();

   exit(0);
}

