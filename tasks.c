#include<stdio.h>
#include<omp.h>

#define MAX_N 50

int value[MAX_N] = {0}, done[MAX_N] = {0};
omp_lock_t locks[MAX_N];

int fib(int n)
{
	if(n<2) return 1;
	
	omp_set_lock(&locks[n]);
	if(!done[n])
	{
		int i,j;
		#pragma omp task shared(i) firstprivate(n)
		i = fib(n-1);
		
		#pragma omp task shared(j) firstprivate(n)
		j = fib(n-2);
		
		#pragma omp taskwait
		value[n] = i+j;
		done[n] = 1;
	}
	omp_unset_lock(&locks[n]);
	return value[n];
}


int main()
{
	int n = 4;
	
	for(int i=0; i<MAX_N; i++)
	{
		omp_init_lock(&locks[i]);
	}
	
	int result;
	#pragma omp parallel
	{
		#pragma omp single
		result = fib(n);
	}
	
	printf("Fibonacci(%d) = %d\n", n, result);
	for(int i=0; i<MAX_N; i++)
	{
		omp_destroy_lock(&locks[i]);
	}

	return 0;
}

