#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv)
{
int ierr, myid, numprocs;

MPI_Init(&argc,&argv);

MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
MPI_Comm_rank(MPI_COMM_WORLD, &myid);

printf("Hello world, I am process %d of %d\n", myid, numprocs);

MPI_Finalize();
}
