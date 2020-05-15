#include<stdio.h>
#include<stdlib.h>
#include "mpi.h"

main(int argc, char* argv[])
{
	int i, *newrank, np, pid, data;
	int n, flag;

	MPI_Init(&argc,&argv);
	MPI_Comm comm_even, comm_split;
   MPI_Group group_world, group_even;
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);

	newrank = (int*)(malloc((np/2)*sizeof(int)));

	for(i=0;i<np/2;i++)
		newrank[i] = i*2;

	MPI_Comm_group(MPI_COMM_WORLD, &group_world);
	MPI_Group_incl(group_world, np/2, newrank, &group_even);
	MPI_Comm_create(MPI_COMM_WORLD, group_even, &comm_even);

	flag = pid % 2;

	MPI_Comm_split(MPI_COMM_WORLD, flag, pid, &comm_split);

	n = pid * 10;
	MPI_Bcast(&n, 1, MPI_INT, 1, comm_split);
	printf("%d %d\n", pid, n);

	MPI_Finalize();
}
