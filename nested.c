#include <stdio.h>
#include <omp.h>

void subprogram() {
    printf(" PRINTING OUTSIDE the Subprogram parallel region.\n");
    #pragma omp parallel
    {
        printf(" PRINTING INSIDE the Subprogram parallel region. SUB Thread ID: %d\n", omp_get_thread_num());
    }
}

int main() {
omp_set_max_active_levels(2);

    printf("PRINTING OUTSIDE the Main program parallel region.\n");

    
    #pragma omp parallel
    {
        printf("PRINTING INSIDE the Main program: parallel region. MAIN Thread ID: %d\n", omp_get_thread_num());
        subprogram();
    }

    subprogram();
    return 0;
}
