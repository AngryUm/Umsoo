#define N 100

int printf(char *, ...);

main()
{
	int sum = 0, i;

	#pragma omp parallel for reduction(+:sum)
	for(i = 0; i < N ;i++)
		sum += i;

	printf("%d\n", sum);
}
