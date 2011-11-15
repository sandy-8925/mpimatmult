/*MPI scatter example 
*/


#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
int ierr,rank,size,dest,source,from,to,count,tag;
long i,counter;
int stat_count, stat_source, stat_tag;
int mat1_rows=10,mat1_cols=10;
int *matrix1,*matrix2;
long matsize = mat1_rows * mat1_cols;
MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

printf("I am process %d of %d\n", rank, size);

source = 0;
matrix1 = (int *) calloc(matsize, sizeof(int));
matrix2 = (int *) calloc(matsize/size, sizeof(int));

if( rank == source )
{
//initialize all data
for(i=0 ; i<matsize ; i++) { *(matrix1 + i) = i%255; }
}

MPI_Scatter(matrix1, matsize/size, MPI_INT, matrix2, matsize/size, MPI_INT, source, MPI_COMM_WORLD);

char *str1,*temp;
sprintf(str1,"Process %d data:\n",rank);
for(counter=0;counter<matsize/size;counter++)
{
sprintf(temp,"%d ",*(matrix2 + counter));
strcat(str1,temp);
}
printf("%s",str1);

printf("Process %d finished\n",rank);


ierr = MPI_Finalize();
free(matrix1);
free(matrix2);

return 0;
}
