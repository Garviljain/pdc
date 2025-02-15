#include<omp.h>
#include<stdio.h>

#define COUNT 10000

int f(int counter, int i)
{
	return (i%10);
}

int main()
{
	int counter = 0;
	#pragma omp parallel for shared(counter)
	for(int i=0; i<COUNT; i++)
	{
		counter += f(counter, i);
	}
	
	printf("Final Value of Counter: %d\n", counter);
	
	

	
	
	return 0;
}
