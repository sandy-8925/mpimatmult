#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
int i,ierr,rank,size,dest,source,from,to,count,tag;
int stat_count, stat_source, stat_tag;
float data[100];
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
count = 100;
tag = 11;
for(i=0;i<100;i++) data[i]=i;
ierr = MPI_Send(data, count, MPI_REAL, to, tag, MPI_COMM_WORLD);
}
else if( rank == dest )
{
tag = MPI_ANY_TAG;
count = 100;
from = MPI_ANY_SOURCE;
ierr = MPI_Recv(data, count, MPI_REAL, from, tag, MPI_COMM_WORLD, &status);
ierr = MPI_Get_count(&status, MPI_REAL, &stat_count);
stat_source = status.MPI_SOURCE;
stat_tag = status.MPI_TAG;
printf("Status of receive: dest=%d source=%d tag=%d count=%d\n", rank, stat_source, stat_tag, stat_count);
}


ierr = MPI_Finalize();
return 0;
}
