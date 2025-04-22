#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUMBER_OF_TESTS 10 // for more reliable average results

int main(int argc, char* argv[]) {
    double *buf;
    int rank, numprocs;
    int n;
    double t1, t2, total_time;
    int j, k, nloop;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    if (numprocs != 2) {
        printf("The number of processes must be two!\n");
        MPI_Finalize();
        return 0;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("\t\tn\tTime [sec]\tRate [Mb/sec]\n");
    }

    for (n = 1; n < 10000000; n *= 2) { // message length doubles
        nloop = 1000000 / n;
        if (nloop < 1) nloop = 1; // just a single loop for long messages

        buf = (double *)malloc(n * sizeof(double));
        if (!buf) {
            printf("Could not allocate message buffer of size %d\n", n);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        total_time = 0.0;

        for (k = 0; k < NUMBER_OF_TESTS; k++) {
            if (rank == 0) {
                t1 = MPI_Wtime();
                for (j = 0; j < nloop; j++) {
                    MPI_Send(buf, n, MPI_DOUBLE, 1, k, MPI_COMM_WORLD);
                }
                t2 = MPI_Wtime();
                total_time += (t2 - t1);
            } else if (rank == 1) {
                for (j = 0; j < nloop; j++) {
                    MPI_Recv(buf, n, MPI_DOUBLE, 0, k, MPI_COMM_WORLD, &status);
                }
            }
        }

        if (rank == 0) {
            double avg_time_per_iteration = total_time / (NUMBER_OF_TESTS * nloop);
            double bandwidth = n * sizeof(double) * 1.0e-6 * 8 / avg_time_per_iteration; // in Mb/sec
            printf("\t\t%d\t%10.8f\t%8.2f\n", n, avg_time_per_iteration, bandwidth);
        }

        free(buf);
    }

    MPI_Finalize();
    return 0;
}
