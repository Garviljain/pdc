#include<omp.h>
#include<stdio.h>

#define N 8

void print_array(const char* label, int* arr, int size)
{
	printf("%s: ", label);
	
	for(int i=0; i<size; i++)
	{
		printf("%d\t", arr[i]);
	}
	printf("\n");
}


int main()
{
	int x[N] ={0}, y[N] = {0};
	omp_set_num_threads(N);

	#pragma omp parallel
	{
		#pragma omp for nowait
		for(int i=0; i<N; i++)
		{
			x[i] = i*2;
			printf("Thread %d computed x[%d] = %d\n", omp_get_thread_num(), i, x[i]);
		}
		
		#pragma omp for
		for(int i=0; i<N-1; i++)
		{
			y[i] = x[i] + 1;
			printf("Thread %d computed y[%d] = %d\n", omp_get_thread_num(), i, y[i]);
		}
		
	}
	
	printf("\nFinal Arrays : \n");
	print_array("x", x, N);
	print_array("y", y, N);
	

	
	
	return 0;
}
