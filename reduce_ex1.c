#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "mpi.h"

long millisecond()
{
    struct timeval tb ;
    gettimeofday(&tb, 0);

    return(1000*tb.tv_sec + tb.tv_usec/1000);
}

main(int argc, char *argv[])
{
   long N, i, start, end;
   int np, pid, dest, tag = 0;
   double dx, x, area, sum;
   char *c;
   MPI_Status status;

   N = 100; // default value
   if (argc == 2) N = (long)strtol(argv[1], &c, 10);

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   area = 0.0;
   dx = 2.0/(double)N;
   dest = 0;
   x = ((double)pid/np) * 2;

   start = millisecond();

   for (i=0; i<N/np; i++) {
      x += dx;
      sum += (x/((x*x+2)*(x*x+2)*(x*x+2))) * dx;
   }

   MPI_Reduce(&sum, &area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   end = millisecond();

   if(pid == 0){
      printf("area: %5.10f\n", area);
      printf("elapsed time: %ld milliseconds\n", end-start);
      printf("GFLOPS: %5.2f\n", (12.0*N)/((end-start)*1000000));
   }

   MPI_Finalize();

   exit(0);
}
