/*
MPI matrix multiplication version 2
Here both A and B are scattered. To use MPI scatter for matrix B, it first needs to be transposed.
Since matrix B is randomly generated for now, as long as the dimensions are equal, it's OK to assume that B has already been transposed.
if reading from a file or using arbitrary matrix dimensions or both, must transpose B and scatter so that columns of B are scattered among different MPI processes. 
*/
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define MATB_EXCHANGE_TAG 5

int rank;

void debugprintf(char *string)
{
printf("Process %d: %s\n",rank,string);
}

int main(int argc, char **argv)
{
int ierr,size,source;
long i,counter;
int mat1_rows=1000,mat1_cols=1000;
int mat2_rows=1000,mat2_cols=1000;
int resultmat_rows=mat1_rows,resultmat_cols=mat2_cols;
int *matrix1,*matrix2,*resultmatrix,*resultmat_data,*mat1_data=NULL,*mat2_data=NULL;
long mat1size = mat1_rows * mat1_cols;
long mat2size = mat2_rows * mat2_cols;
long resultmatsize = resultmat_rows * resultmat_cols;
char outstring[10000],temp[20];
MPI_Request request;
MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

sprintf(outstring,"I am process %d of %d", rank, size);
debugprintf(outstring);

source = 0;
matrix1 = (int *) calloc(mat1size/size, sizeof(int));
matrix2 = (int *) calloc(mat2size/size, sizeof(int));
resultmatrix = (int *) calloc(resultmatsize/size, sizeof(int));

if( rank == source )
{
//initialize all data
resultmat_data = (int *) calloc(resultmatsize, sizeof(int));
mat1_data = (int *) calloc(mat1size,sizeof(int));
mat2_data = (int *) calloc(mat2size,sizeof(int));
for(i=0 ; i<mat1size ; i++) { *(mat1_data + i) = i; }
for(i=0 ; i<mat2size ; i++) { *(mat2_data + i) = i; }
}
else
{
}

//distribute data
MPI_Scatter(mat1_data, mat1size/size, MPI_INT, matrix1, mat1size/size, MPI_INT, source, MPI_COMM_WORLD);
MPI_Scatter(mat2_data, mat2size/size, MPI_INT, matrix2, mat2size/size, MPI_INT, source, MPI_COMM_WORLD);
//MPI_Bcast(matrix2, mat2size, MPI_INT, source, MPI_COMM_WORLD);

if(rank == source)
{
free(mat1_data);
free(mat2_data);
mat1_data=mat2_data=NULL;
}

long counter1,counter2,counter3,counter4;

mat2_data = (int *) calloc(mat2size/size, sizeof(int));

for(counter4=0; counter4<size; counter4++)
{
int portion = (rank - counter4)%rank;
for(counter1=0 ; counter1<resultmat_rows/size ; counter1++)
{
for(counter2=0 ; counter2<resultmat_cols/size ; counter2++)
{
int tempsum = 0;
for(counter3=0 ; counter3<mat1_cols ; counter3++)
{ tempsum += *(matrix1 + counter1*mat1_cols + counter3) * *(matrix2 + counter2*mat2_cols + counter3); }
*(resultmatrix + counter1*resultmat_cols + counter2+((portion*resultmat_cols)/size)) = tempsum;
}
}
//exchange data if required
if(counter4<size-1)
{
MPI_Barrier(MPI_COMM_WORLD);
//do non-blocking send and receive
bcopy(matrix2, mat2_data, mat2size*sizeof(int)/size);
MPI_Isend(mat2_data, mat2size/size, MPI_INT, (rank+1)%rank, MATB_EXCHANGE_TAG, MPI_COMM_WORLD, &request);
MPI_Recv(matrix2, mat2size/size, MPI_INT, (rank-1)%rank, MATB_EXCHANGE_TAG, MPI_COMM_WORLD, &status);
}
}

sprintf(outstring,"Matrix multiplication finished\n");
debugprintf(outstring);

MPI_Barrier(MPI_COMM_WORLD);
free(mat2_data);
mat2_data=NULL;
//return result to process rank 0
//note: resultmatrix is transpose of actual answer. to get original answer, need to transpose in master process (rank 0)
MPI_Gather(resultmatrix , resultmatsize/size, MPI_INT, resultmat_data, resultmatsize/size, MPI_INT, source, MPI_COMM_WORLD);

/*
if(rank==0)
{
//print result matrix to screen
sprintf(outstring,"Resultmatrix data:\n");
for(counter1=0 ; counter1<resultmat_rows ; counter1++)
{
for(counter2=0; counter2<resultmat_cols; counter2++)
{
sprintf(temp,"%d ",*(resultmat_data + (counter1*resultmat_cols) + counter2));
strcat(outstring,temp);
}
strcat(outstring,"\n");
}
debugprintf(outstring);
}
*/

sprintf(outstring,"finished");
debugprintf(outstring);

if(rank == source)
{ free(resultmat_data); }

ierr = MPI_Finalize();
free(matrix1);
free(matrix2);
free(resultmatrix);

return 0;
}
