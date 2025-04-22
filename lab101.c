#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_TESTS 100

int main(int argc, char *argv[]) {
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size < 2) {
    	if (rank == 0) printf("This program requires at least 2 processes.\n");
    	MPI_Finalize();
    	return 0;
	}

	int message_sizes[] = {1, 10, 100, 1000};
	int num_sizes = sizeof(message_sizes) / sizeof(message_sizes[0]);

	if (rank == 0)
    	printf("Message Size (doubles)\tAvg. Latency per round-trip [ms]\n");

	for (int s = 0; s < num_sizes; ++s) {
    	int count = message_sizes[s];
    	double *buffer = (double *)malloc(sizeof(double) * count);
    	for (int i = 0; i < count; i++) buffer[i] = 1.0 * i;

    	double total_time = 0.0;

    	for (int test = 0; test < NUMBER_OF_TESTS; ++test) {
        	MPI_Barrier(MPI_COMM_WORLD);
        	double start_time = MPI_Wtime();

        	if (rank == 0) {
            	MPI_Send(buffer, count, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            	MPI_Recv(buffer, count, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        	} else {
            	MPI_Recv(buffer, count, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            	int next = (rank + 1) % size;
            	MPI_Send(buffer, count, MPI_DOUBLE, next, 0, MPI_COMM_WORLD);
        	}

        	double end_time = MPI_Wtime();
        	if (rank == 0)
            	total_time += (end_time - start_time);
    	}

    	if (rank == 0) {
        	double avg_time_ms = (total_time / NUMBER_OF_TESTS) * 1000.0;
        	printf("%d\t\t\t%.3f\n", count, avg_time_ms);
    	}

    	free(buffer);
	}

	MPI_Finalize();
	return 0;
}
