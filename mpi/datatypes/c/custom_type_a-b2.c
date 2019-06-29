#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank;
    int array[8][8];
    //TODO: Declare a variable storing the MPI datatype
    MPI_Datatype columntype;
    int i, j;
    int displs[4];
    int counts[4];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize arrays
    if (rank == 0) {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                array[i][j] = (i + 1) * 10 + j + 1;
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                array[i][j] = 0;
            }
        }
    }

    if (rank == 0) {
        printf("Data in rank 0\n");
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

	for(i=0;i<4;i++){
	counts[i] = i + 1;
	displs[i] = i + 2 * i * 8; 
	}


    //TODO: Create datatype that describes one column. Use MPI_Type_vector.
	 MPI_Type_indexed(4, counts, displs, MPI_INT, &columntype);
	 MPI_Type_commit(&columntype);
    //TODO: Send first column of matrix form rank 0 to rank 1
	if(rank == 0){
	MPI_Send(array, 1, columntype, 1, 1, MPI_COMM_WORLD);
	}
	if(rank == 1){
	MPI_Recv(array, 1, columntype, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
    //TODO: free datatype

    // Print out the result on rank 1
    // The application is correct if the first column has the values of rank 0
    if (rank == 1) {
        printf("Received data\n");
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Type_free(&columntype);   	
    MPI_Finalize();

    return 0;
}
