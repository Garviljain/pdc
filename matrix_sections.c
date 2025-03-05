#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include<time.h>
#define SIZE 100

int mat[SIZE][SIZE], vec[SIZE], result[SIZE];
int privateVar; 
#pragma omp threadprivate(privateVar)

void initialize() {
srand(time(NULL));
   for (int i = 0; i < SIZE; i++) {
       vec[i] = rand() % 10; 
       for (int j = 0; j < SIZE; j++) {
           mat[i][j] = rand() % 10; 
       }
   }
}

void matrix_vector_multiplication() {
   #pragma omp parallel
   {
       #pragma omp single
       printf("Parallel computation started with %d threads.\n", omp_get_num_threads());

       #pragma omp for reduction(+:result[:SIZE])
       for (int i = 0; i < SIZE; i++) {
           result[i] = 0;
           for (int j = 0; j < SIZE; j++) {
               result[i] += mat[i][j] * vec[j];
           }
       }
   }
}

void compute_sum_and_max() {
   int sum = 0, max_val = 0;

   #pragma omp parallel sections
   {
       #pragma omp section
       {
           for (int i = 0; i < SIZE; i++) {
               sum += result[i];
           }
           printf("Sum of all elements in result[]: %d\n", sum);
       }

       #pragma omp section
       {
           max_val = result[0];
           for (int i = 1; i < SIZE; i++) {
               if (result[i] > max_val) {
                   max_val = result[i];
               }
           }
           printf("Maximum value in result[]: %d\n", max_val);
       }
   }
}

int main() {
   srand(time(NULL));  // Seed for reproducibility

   initialize();
   matrix_vector_multiplication();
   compute_sum_and_max();

   return 0;
}

