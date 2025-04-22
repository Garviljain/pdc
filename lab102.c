#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int rank, size;
	int n;

	MPI_Init(&argc, &argv);          	 
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


    
	if (rank == 0) {
    	n = 2;
    	printf("Process 0 initializing n = %d\n", n);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);


	int *send_data = NULL;
	if (rank == 0) {
    	send_data = (int *)malloc(sizeof(int) * n * size);
    	for (int i = 0; i < n * size; ++i) {
        	send_data[i] = i + 1;
    	}
	}

	int *recv_data = (int *)malloc(sizeof(int) * n);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Scatter(send_data, n, MPI_INT, recv_data, n, MPI_INT, 0, MPI_COMM_WORLD);


	printf("Process %d received: ", rank);
	for (int i = 0; i < n; ++i) {
    	recv_data[i] *= 2;
    	printf("%d ", recv_data[i]);
	}
	printf("\n");


	int *gathered_data = NULL;
	if (rank == 0) {
    	gathered_data = (int *)malloc(sizeof(int) * n * size);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(recv_data, n, MPI_INT, gathered_data, n, MPI_INT, 0, MPI_COMM_WORLD);

 
	if (rank == 0) {
    	printf("Final gathered array in process 0: ");
    	for (int i = 0; i < n * size; ++i) {
        	printf("%d ", gathered_data[i]);
    	}
    	printf("\n");
	}


	if (send_data) free(send_data);
	if (gathered_data) free(gathered_data);
	free(recv_data);

	MPI_Finalize();
	return 0;
}
