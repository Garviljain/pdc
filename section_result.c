#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include<time.h>
#define SIZE 1000000

double computeA() {
   return 10.0;
}
double computeB() {
   return 20.0;
}

double privateVar;
#pragma omp threadprivate(privateVar)

int main() {
   srand(time(NULL));
   int vec1[SIZE], vec2[SIZE];
   long long dot_product_parallel = 0, dot_product_serial = 0;
   double sharedVar = 0.0, section_result = 0.0;


   for (int i = 0; i < SIZE; i++) {
       vec1[i] = rand() % 100 + 1;
       vec2[i] = rand() % 100 + 1;
   }


   double start_time_serial = omp_get_wtime();
   for (int i = 0; i < SIZE; i++) {
       dot_product_serial += vec1[i] * vec2[i];
   }
   double end_time_serial = omp_get_wtime();

   double start_time_parallel = omp_get_wtime();
   #pragma omp parallel reduction(+:dot_product_parallel, section_result)
   {
       privateVar = omp_get_thread_num() + 1.0;

       #pragma omp single
       {
           sharedVar = 5.0;
           printf("Shared variable set by thread %d\n", omp_get_thread_num());
       }

       #pragma omp for
       for (int i = 0; i < SIZE; i++) {
           dot_product_parallel += vec1[i] * vec2[i];
       }

       #pragma omp sections
       {
           #pragma omp section
           {
               double resA = computeA() * privateVar * sharedVar;
               printf("Section computed by thread %d: %f\n", omp_get_thread_num(), resA);
               section_result += resA;
           }

           #pragma omp section
           {
               double resB = computeB() * privateVar * sharedVar;
               printf("Section computed by thread %d: %f\n", omp_get_thread_num(), resB);
               section_result += resB;
           }
       }
   }
   double end_time_parallel = omp_get_wtime();

   printf("Dot Product (Serial): %lld\n", dot_product_serial);
   printf("Dot Product (Parallel): %lld\n", dot_product_parallel);
   printf("Section Result: %f\n", section_result);
   printf("Serial Execution Time: %f seconds\n", end_time_serial - start_time_serial);
   printf("Parallel Execution Time: %f seconds\n", end_time_parallel - start_time_parallel);

   return 0;
}
