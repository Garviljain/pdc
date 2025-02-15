#include<stdio.h>
#include<omp.h>

#define NUM_THREADS 8

omp_lock_t lockA, lockB;
int a = 0, b=0;

void deadlock_example(){
	#pragma omp parallel num_threads(NUM_THREADS)
	{	
		int tid = omp_get_thread_num();
		
		if(tid == 0)
		{	
			omp_set_lock(&lockA);
			printf("Thread 0 locked A\n");
			
			a+=1;
			
			omp_set_lock(&lockB);
			printf("Thread 0 locked B\n");
			
			b+=a;
			omp_unset_lock(&lockB);
			omp_unset_lock(&lockA);
		}
		else
		{
			omp_set_lock(&lockB);
			printf("Thread 1 locked B\n");
			
			b+=1;
			
			omp_set_lock(&lockA);
			printf("Thread 1 locked A\n");
			
			a+=b;
			omp_unset_lock(&lockA);
			omp_unset_lock(&lockB);
		}
	}
}


void fixed_example(){
	#pragma omp parallel num_threads(NUM_THREADS)
	{	
		int tid = omp_get_thread_num();
		
		
			omp_set_lock(&lockA);
			printf("Thread %d locked A\n", tid);
			
			a+=1;
			
			omp_set_lock(&lockB);
			printf("Thread %d locked B\n", tid);
			
			b+=a;
			omp_unset_lock(&lockB);
			omp_unset_lock(&lockA);
	}
}

int main()
{
	omp_init_lock(&lockA);
	omp_init_lock(&lockB);
	
	printf("Demonstrating deadlock (this may hang!)...\n");
	fixed_example();
	omp_destroy_lock(&lockA);
	omp_destroy_lock(&lockB);
	printf("Final Values: a : %d , b : %d \n", a, b);
	return 0;
}

