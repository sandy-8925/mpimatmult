#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
int i,ierr,rank,size,dest,source,from,to,count,tag;
int stat_count, stat_source, stat_tag;
int mat1_rows=1000,mat1_cols=1000;
int *matrix1;
MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

printf("I am process %d of %d\n", rank, size);
dest = size - 1;
source = 0;

if( rank == source )
{
to = dest;
count = SIZE;
tag = 11;
matrix1 = (int *) calloc(mat1_rows*mat1_cols, sizeof(int));
for(i=0 ; i<SIZE ; i++) { *(data + i) = i; }
for(to=1 ; to < size ; to++)
    ierr = MPI_Send(data, count, MPI_REAL, to, tag, MPI_COMM_WORLD);
}
else
{
tag = MPI_ANY_TAG;
count = SIZE;
from = MPI_ANY_SOURCE;
matrix1 = (int *) calloc(SIZE, sizeof(int));
ierr = MPI_Recv(data, count, MPI_REAL, from, tag, MPI_COMM_WORLD, &status);
ierr = MPI_Get_count(&status, MPI_REAL, &stat_count);
stat_source = status.MPI_SOURCE;
stat_tag = status.MPI_TAG;
printf("Process %d: Status of receive: dest=%d source=%d tag=%d count=%d\n", rank, rank, stat_source, stat_tag, stat_count);
}


ierr = MPI_Finalize();
return 0;
}
