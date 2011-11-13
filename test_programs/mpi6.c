#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
int ierr,rank,size,dest,source,from,to,count,tag;
long i;
int stat_count, stat_source, stat_tag;
int mat1_rows=10000,mat1_cols=10000;
int *matrix1;
long matsize = mat1_rows * mat1_cols;
MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

printf("I am process %d of %d\n", rank, size);
source = 0;

matrix1 = (int *) calloc(matsize, sizeof(int));

if( rank == source )
{
for(i=0 ; i<matsize ; i++) { *(matrix1 + i) = i%255; }
MPI_Bcast(matrix1, matsize, MPI_INT, source, MPI_COMM_WORLD);
}
else
{
MPI_Bcast(matrix1, matsize, MPI_INT, source, MPI_COMM_WORLD);
}
printf("Process %d finished\n",rank);
free(matrix1);
ierr = MPI_Finalize();
return 0;
}
