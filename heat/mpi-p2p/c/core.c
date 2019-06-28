/* Main solver routines for heat equation solver */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>

#include "heat.h"

/* Exchange the boundary values */
void exchange(field *temperature, parallel_data *parallel)
{
	MPI_Status status;
    /* implement halo exchange */
 	//Send to the up, receive from down

	MPI_Sendrecv(temperature->data[1],temperature->ny, MPI_DOUBLE, parallel->nup, 1, temperature->data[temperature->nx-1],temperature->ny, MPI_DOUBLE, parallel->ndown, 1, MPI_COMM_WORLD, &status);

    // Send to the down, receive from up

	MPI_Sendrecv(temperature->data[temperature->nx-1],temperature->ny, MPI_DOUBLE, parallel->ndown, 2, temperature->data[1],temperature->ny, MPI_DOUBLE, parallel->nup, 2, MPI_COMM_WORLD, &status);


}


/* Update the temperature values using five-point stencil */
void evolve(field *curr, field *prev, double a, double dt)
{
    int i, j;
    double dx2, dy2;

    /* Determine the temperature field at next time step
     * As we have fixed boundary conditions, the outermost gridpoints
     * are not updated. */
    dx2 = prev->dx * prev->dx;
    dy2 = prev->dy * prev->dy;
    for (i = 1; i < curr->nx + 1; i++) {
        for (j = 1; j < curr->ny + 1; j++) {
            curr->data[i][j] = prev->data[i][j] + a * dt *
                               ((prev->data[i + 1][j] -
                                 2.0 * prev->data[i][j] +
                                 prev->data[i - 1][j]) / dx2 +
                                (prev->data[i][j + 1] -
                                 2.0 * prev->data[i][j] +
                                 prev->data[i][j - 1]) / dy2);
        }
    }
}


