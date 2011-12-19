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
#define ERR_NOT_ENOUGH_ARGUMENTS -5;

//process rank. made global for debugprintf function
int rank;

void debugprintf(char *string)
{
printf("Process %d: %s\n",rank,string);
}

int main(int argc, char **argv)
{
int ierr,size,source;
long i,counter;
int mat1_rows,mat1_cols;
int mat2_rows,mat2_cols;
mat1_rows = 1000;
mat2_cols = 1000;
int comm_dim = 1000;
mat1_cols = mat2_rows = comm_dim;
int resultmat_rows=mat1_rows,resultmat_cols=mat2_cols;
int *matrix1,*matrix2,*resultmatrix,*resultmat_data,*mat1_data=NULL,*mat2_data=NULL;
long mat1size = mat1_rows * mat1_cols;
long mat2size = mat2_rows * mat2_cols;
long resultmatsize = resultmat_rows * resultmat_cols;
char outstring[10000],temp[20];
MPI_Request request;
MPI_Status status;
double start_time,end_time;

if(argc<4)
{
printf("Not enough arguments\n");
return ERR_NOT_ENOUGH_ARGUMENTS;
}
else
{
mat1_rows = atoi(argv[1]);
comm_dim = atoi(argv[2]);
mat1_cols = mat2_rows = comm_dim;
mat2_cols = atoi(argv[3]);
resultmat_rows = mat1_rows;
resultmat_cols = mat2_cols;
mat1size = mat1_rows * mat1_cols;
mat2size = mat2_rows * mat2_cols;
resultmatsize = resultmat_rows * resultmat_cols;
}

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
//TODO: transpose matrix 2. right now, we assume that it is already transposed
sprintf(outstring, "Finished generating data");
debugprintf(outstring);
}

MPI_Barrier(MPI_COMM_WORLD);

sprintf(outstring,"About to distribute data");
debugprintf(outstring);

//distribute data
MPI_Scatter(mat1_data, mat1size/size, MPI_INT, matrix1, mat1size/size, MPI_INT, source, MPI_COMM_WORLD);
MPI_Scatter(mat2_data, mat2size/size, MPI_INT, matrix2, mat2size/size, MPI_INT, source, MPI_COMM_WORLD);

if( rank == source )
{
sprintf(outstring,"Matrix 1 memory usage per process: %f bytes \n",mat1size*sizeof(int)/size);
debugprintf(outstring);
sprintf(outstring,"Matrix 2 memory usage per process: %f bytes \n",mat2size*sizeof(int)/size);
debugprintf(outstring);
}

MPI_Barrier(MPI_COMM_WORLD);

if(rank == source)
{
free(mat1_data);
free(mat2_data);
mat1_data=mat2_data=NULL;
}

long counter1,counter2,counter3,counter4;

mat2_data = (int *) calloc(mat2size/size, sizeof(int));

sprintf(outstring,"Finished distributing data and about to start matrix multiplication");
debugprintf(outstring);

MPI_Barrier(MPI_COMM_WORLD);
start_time = MPI_Wtime();

for(counter4=0; counter4<size; counter4++)
{
sprintf(outstring, "Started round %d", counter4);
//debugprintf(outstring);
int portion = (rank - counter4)%size;
for(counter1=0 ; counter1<resultmat_rows/size ; counter1++)
{
for(counter2=0 ; counter2<resultmat_cols/size ; counter2++)
{
int tempsum = 0;
for(counter3=0 ; counter3<mat1_cols ; counter3++)
//it is assumed that matrix 2 stored columnwise instead of rowwise
{ tempsum += *(matrix1 + counter1*mat1_cols + counter3) * *(matrix2 + counter2*mat2_rows + counter3); }
*(resultmatrix + counter1*resultmat_cols + portion*resultmat_cols/size + counter2  ) = tempsum;
}
}
sprintf(outstring, "Finished round %d", counter4);
//debugprintf(outstring);
//exchange data if required
if(counter4 < size-1)
{
MPI_Barrier(MPI_COMM_WORLD);
//do non-blocking send and blocking receive
bcopy(matrix2, mat2_data, mat2size*sizeof(int)/size);
MPI_Isend(mat2_data, mat2size/size, MPI_INT, (rank+1)%size, MATB_EXCHANGE_TAG, MPI_COMM_WORLD, &request);
MPI_Recv(matrix2, mat2size/size, MPI_INT, (rank-1)%size, MATB_EXCHANGE_TAG, MPI_COMM_WORLD, &status);
}
}

MPI_Barrier(MPI_COMM_WORLD);
end_time = MPI_Wtime();

sprintf(outstring,"MPI walltime: %f\n", end_time - start_time);
debugprintf(outstring);

sprintf(outstring,"Matrix multiplication finished\n");
debugprintf(outstring);

free(mat2_data);
mat2_data=NULL;
//return result to process rank 0
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

//MPI_Barrier(MPI_COMM_WORLD);
/*
if(rank == source)
{ free(resultmat_data); }
free(matrix1);
free(matrix2);
free(resultmatrix);
*/
ierr = MPI_Finalize();

/*if(ierr != MPI_SUCCESS)
{
debugprintf("MPI_Finalize did not return MPI_SUCCESS!\n");
}*/
printf("After MPI_Finalize\n");
return 0;
}
