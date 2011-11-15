/*MPI scatter example 
*/
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

int rank;

void debugprintf(char *string)
{
printf("Process %d: %s\n",rank,string);
}

int main(int argc, char **argv)
{
int ierr,size,source;
long i,counter;
int mat1_rows=10,mat1_cols=10;
int mat2_rows=10,mat2_cols=10;
int resultmat_rows=mat1_rows,resultmat_cols=mat2_cols;
int *matrix1,*matrix2,*resultmatrix;
long mat1size = mat1_rows * mat1_cols;
long mat2size = mat2_rows * mat2_cols;
long resultmatsize = resultmat_rows * resultmat_cols;
char outstring[10000];

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

sprintf(outstring,"I am process %d of %d", rank, size);
debugprintf(outstring);

source = 0;
matrix1 = (int *) calloc(mat1size, sizeof(int));
matrix2 = (int *) calloc(mat2size, sizeof(int));
resultmatrix = (int *) calloc(resultmatsize, sizeof(int));

if( rank == source )
{
//initialize all data
for(i=0 ; i<mat1size ; i++) { *(matrix1 + i) = i; }
for(i=0 ; i<mat2size ; i++) { *(matrix2 + i) = i; }
}
else
{
}

//distribute data
//MPI_Scatter(mat1_data, mat1size/size, MPI_INT, matrix1, mat1size/size, MPI_INT, source, MPI_COMM_WORLD);
MPI_Bcast(matrix1, mat1size, MPI_INT, source, MPI_COMM_WORLD);
MPI_Bcast(matrix2, mat2size, MPI_INT, source, MPI_COMM_WORLD);

int counter1,counter2,counter3;

for(counter1 = rank*resultmat_rows/size ; counter1 < (rank+1)*resultmat_rows/size ; counter1++)
{
for(counter2=0 ; counter2<resultmat_cols; counter2++)
{
int tempsum = 0;
for(counter3=0 ; counter3<mat1_cols ; counter3++)
{ tempsum += *(matrix1 + counter1*mat1_cols + counter3) * *(matrix2 + counter3*mat2_cols + counter2); }
*(resultmatrix + counter1*resultmat_cols + counter2) = tempsum;
}
}


//do matrix multiplication


sprintf(outstring,"finished",rank);
debugprintf(outstring);

MPI_Barrier(MPI_COMM_WORLD);
ierr = MPI_Finalize();
free(matrix1);
free(matrix2);
free(resultmatrix);

return 0;
}
