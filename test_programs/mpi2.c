#include <stdio.h>
#include <mpi.h>

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
int *matrix1,*matrix2;
matrix1 = (int *) calloc(mat1_rows*mat1_cols, sizeof(int)); 
}
else
{
}

MPI_Finalize();
return 0;
}
