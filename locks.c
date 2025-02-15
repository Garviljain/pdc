#include<stdio.h>
#include<omp.h>

#define NUM_THREADS 4
#define NUM_ITERATIONS 5

omp_lock_t mylock;
int shared_variable = 0;

void some_calculation(int thread_id)
{
	printf("Thread %d is modifying shared_variable\n", thread_id);
	shared_variable += thread_id;
}

int main()
{
	omp_init_lock(&mylock);
	
	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int mytid = omp_get_thread_num();
		
		omp_set_lock(&mylock);
		some_calculation(mytid);
		printf("Thread %d updated shared_variable to %d\n", mytid, shared_variable);
		omp_unset_lock(&mylock);
	}
	
	omp_destroy_lock(&mylock);
	printf("Final value of shared_variable : %d\n", shared_variable);
	return 0;
}
