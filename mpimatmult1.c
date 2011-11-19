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
int mat1_rows=1000,mat1_cols=1000;
int mat2_rows=1000,mat2_cols=1000;
int resultmat_rows=mat1_rows,resultmat_cols=mat2_cols;
int *matrix1,*matrix2,*resultmatrix,*resultmat_data,*mat1_data;
long mat1size = mat1_rows * mat1_cols;
long mat2size = mat2_rows * mat2_cols;
long resultmatsize = resultmat_rows * resultmat_cols;
char outstring[10000],temp[20];

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

sprintf(outstring,"I am process %d of %d", rank, size);
debugprintf(outstring);

source = 0;
matrix1 = (int *) calloc(mat1size/size, sizeof(int));
matrix2 = (int *) calloc(mat2size, sizeof(int));
resultmatrix = (int *) calloc(resultmatsize/size, sizeof(int));

if( rank == source )
{
//initialize all data
resultmat_data = (int *) calloc(resultmatsize, sizeof(int));
mat1_data = (int *) calloc(mat1size,sizeof(int));
for(i=0 ; i<mat1size ; i++) { *(mat1_data + i) = i; }
for(i=0 ; i<mat2size ; i++) { *(matrix2 + i) = i; }
}
else
{
}

//distribute data
//MPI_Bcast(matrix1, mat1size, MPI_INT, source, MPI_COMM_WORLD);
MPI_Scatter(mat1_data, mat1size/size, MPI_INT, matrix1, mat1size/size, MPI_INT, source, MPI_COMM_WORLD);
MPI_Bcast(matrix2, mat2size, MPI_INT, source, MPI_COMM_WORLD);

if(rank == source)
{ free(mat1_data); }

long counter1,counter2,counter3;

//do matrix multiplication
for(counter1 = 0 ; counter1 < resultmat_rows/size ; counter1++)
{
for(counter2=0 ; counter2<resultmat_cols; counter2++)
{
int tempsum = 0;
for(counter3=0 ; counter3<mat1_cols ; counter3++)
{ tempsum += *(matrix1 + counter1*mat1_cols + counter3) * *(matrix2 + counter3*mat2_cols + counter2); }
*(resultmatrix + counter1*resultmat_cols + counter2) = tempsum;
}
}

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

if(rank == source)
{ free(resultmat_data); }

ierr = MPI_Finalize();
free(matrix1);
free(matrix2);
free(resultmatrix);

return 0;
}
