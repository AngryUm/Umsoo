#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mpi.h"

main(int argc, char *argv[])
{
   int N = 24;
   int np, pid, i;
   int A[24],B[6];
   char *c;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   for(i=0;i<N;i++)
        A[i]=i+1;

   MPI_Scatter(&A, N/np, MPI_INT, &B, N/np, MPI_INT, 0, MPI_COMM_WORLD);

   for(i=0;i<6;i++){
      printf("%d : %d\n", pid, B[i]);
   }

   MPI_Finalize();

   exit(0);
}

