#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int i, myid, ntasks;
    int msgsize = 10000000;
    int *message;
    int *receiveBuffer;
    MPI_Status status;

    MPI_Request myreq_send;
    MPI_Request myreq_recv;

    double t0, t1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    /* Allocate message buffers */
    message = (int *)malloc(sizeof(int) * msgsize);
    receiveBuffer = (int *)malloc(sizeof(int) * msgsize);
    /* Initialize message */
    for (i = 0; i < msgsize; i++) {
        message[i] = myid;
    }

    /* Start measuring the time spent in communication */
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();

    /* TODO start */
    /* Send and receive messages as defined in exercise */
    if (myid < ntasks - 1) {
	MPI_Isend(message, msgsize, MPI_INT, myid+1, myid+1, MPI_COMM_WORLD, &myreq_send);
	MPI_Wait(&myreq_send, MPI_STATUSES_IGNORE);

        printf("Sender: %d. Sent elements: %d. Tag: %d. Receiver: %d\n",
               myid, msgsize, myid + 1, myid + 1);
    }

    if (myid > 0) {
	MPI_Irecv(receiveBuffer, msgsize, MPI_INT, myid-1, myid, MPI_COMM_WORLD, &myreq_recv);
 	MPI_Wait(&myreq_recv, MPI_STATUSES_IGNORE);
        printf("Receiver: %d. first element %d.\n",
               myid, receiveBuffer[0]);
    }
    /* TODO end */

    /* Finalize measuring the time and print it out */
    t1 = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    fflush(stdout);

    printf("Time elapsed in rank %2d: %6.3f\n", myid, t1 - t0);

    free(message);
    free(receiveBuffer);
    MPI_Finalize();
    return 0;
}
