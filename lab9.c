#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char *argv[]) {
    int numprocs, rank, tag = 100, msg_size = 64;
    char *buf;
    MPI_Status status;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if (numprocs != 3) {
        printf("This program requires exactly three processes!\n");
        MPI_Finalize();
        return 0;
    }


    printf("MPI process %d started...\n", rank);
    fflush(stdout);


    while (msg_size <= 1000000) {
        buf = (char *)malloc(msg_size * sizeof(char));


        if (rank == 0) {
            // Process 0 sends to Process 1, then receives from Process 2
            MPI_Send(buf, msg_size, MPI_BYTE, 1, tag, MPI_COMM_WORLD);
            printf("Process 0 sent message of size %d to Process 1\n", msg_size);
            fflush(stdout);


            MPI_Recv(buf, msg_size, MPI_BYTE, 2, tag, MPI_COMM_WORLD, &status);
            printf("Process 0 received message of size %d from Process 2\n", msg_size);
            fflush(stdout);
        }
        else if (rank == 1) {
            // Process 1 receives from Process 0, then sends to Process 2
            MPI_Recv(buf, msg_size, MPI_BYTE, 0, tag, MPI_COMM_WORLD, &status);
            printf("Process 1 received message of size %d from Process 0\n", msg_size);
            fflush(stdout);


            MPI_Send(buf, msg_size, MPI_BYTE, 2, tag, MPI_COMM_WORLD);
            printf("Process 1 sent message of size %d to Process 2\n", msg_size);
            fflush(stdout);
        }
        else if (rank == 2) {
            // Process 2 receives from Process 1, then sends to Process 0
            MPI_Recv(buf, msg_size, MPI_BYTE, 1, tag, MPI_COMM_WORLD, &status);
            printf("Process 2 received message of size %d from Process 1\n", msg_size);
            fflush(stdout);


            MPI_Send(buf, msg_size, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
            printf("Process 2 sent message of size %d to Process 0\n", msg_size);
            fflush(stdout);
        }


        free(buf);
        msg_size *= 2;  // Double the message size
    }


    MPI_Finalize();
    return 0;
}





