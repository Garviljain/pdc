#include <stdio.h>
#include <omp.h>

double computeA() {
   return 10.0;
}
double computeB() {
   return 20.0;
}

int main() {
   double res = 0.0;
   double sharedVar = 0.0;

   #pragma omp parallel
   {
    
       #pragma omp single
       {
           sharedVar = 5.0;
           printf("Shared variable initialized by thread %d\n", omp_get_thread_num());
       }

       #pragma omp sections reduction(+:res)
       {
           #pragma omp section
           {
               double resA = computeA() * sharedVar;
               printf("Section 1 computed by thread %d: %f\n", omp_get_thread_num(), resA);
               res += resA;
           }
           #pragma omp section
           {
               double resB = computeB() * sharedVar;
               printf("Section 2 computed by thread %d: %f\n", omp_get_thread_num(), resB);
               res += resB;
           }
       }
   }

   printf("Final result: %f\n", res);
   return 0;
}

