#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

long millisecond()
{
    struct timeval tb ;
    gettimeofday(&tb, 0);

    return(1000*tb.tv_sec + tb.tv_usec/1000);
}

main(int argc, char *argv[])
{
   long N, i, start, end;
   double dx, x, area;
   char *c;
   int id, k;

   N = 1000000; // default value
   if (argc == 2) N = (long)strtol(argv[1], &c, 10);

   start = millisecond();

   area = 0.0;
   dx = 2.0/(double)N;
   #pragma omp parallel private(i,x)
   {
      k = omp_get_num_threads();
      id = omp_get_thread_num();
      x = ((double)id/k) * 2 ;
      #pragma omp for reduction(+:area)
      for (i=0; i<N; i++) {
         x += dx;
         area += (x/((x*x+2)*(x*x+2)*(x*x+2))) * dx; 
      }
   }
   end = millisecond();

   printf("area: %5.10f\n", area);
   printf("elapsed time: %ld milliseconds\n", end-start);
   printf("GFLOPS: %5.2f\n", (12.0*N)/((end-start)*1000000));
   
   exit(0);
}
