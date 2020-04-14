#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<math.h>
#include"mpi.h"

long millisecond()
{
    struct timeval tb ;
    gettimeofday(&tb, 0);

    return(1000*tb.tv_sec + tb.tv_usec/1000);
}

main(int argc, char *argv[])
{
   int N=200000000;
   double *A,*B,*C,*D;
   double local_dotprod, dotprod, temp;
   long i, start, end;
   int np, pid;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   temp = N/np;

   if(pid == 0){
      A = malloc(sizeof(double)*N);
      B = malloc(sizeof(double)*N);

      for (i=0; i<N; i++) {
         A[i] = i*0.000000000001;
         B[i] = i*0.000000000002 + 3.0;
      }
   }

   C = malloc(sizeof(double)*temp);
   D = malloc(sizeof(double)*temp);
    
   start = millisecond();

   MPI_Scatter(A, temp, MPI_DOUBLE, C, temp, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Scatter(B, temp, MPI_DOUBLE, D, temp, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
   local_dotprod = 0.0;
   for(i=0; i<temp; i++){
      local_dotprod += C[i] * D[i]; 
   }

   MPI_Reduce(&local_dotprod, &dotprod, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  
   end = millisecond();   

   if(pid == 0)
      printf("결과값 : %lf 실행시간 : %ld\n", dotprod, end-start);

   MPI_Finalize();

   exit(0);
}

