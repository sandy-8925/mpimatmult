#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
int myrank,numprocs;
MPI_Init(&argc,&argv);

MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

if(myrank==0)
{
/*
//read data from file
int inputfildesc;
char *filename;
*/
unsigned int mat1_rows,mat1_cols,mat2_rows,mat2_cols;
mat1_rows = mat1_cols = 1000;
int *matrix1,*matrix2;
int *mstart,*mend,*temp;
int data = 10000;

matrix1 = (int *) calloc(mat1_rows*mat1_cols, sizeof(int));

//fill matrix1 with random data
mend = matrix1 + mat1_rows*mat1_cols;
temp = matrix1;
while(temp < mend)
{ *(temp++)= random()%256; }

//send small amount of data to another MPI process
MPI_Send(&data,1,datatype,rank,1,MPI_COMM_WORLD);

//send whole of matrix1 to another MPI process

}
else
{
}

MPI_Finalize();
return 0;
}
