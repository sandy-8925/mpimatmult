#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <mpi.h>

//TODO: change #define symbols to integer variables and initialize using command line args

/*
#define mat_comm_dim 1000
#define matA_rows 1000
#define matA_cols mat_comm_dim
#define matB_rows mat_comm_dim
#define matB_cols 1000
#define matC_rows matA_rows
#define matC_cols matB_cols
*/

int main(int argc, char **argv)
{
int mat_comm_dim = 1000;
int matA_rows = 1000,matA_cols = mat_comm_dim,matB_rows = mat_comm_dim,matB_cols = 1000;
int matC_rows = matA_rows, matC_cols = matB_cols;

long matAsize = matA_rows * matA_cols;
long matBsize = matB_rows * matB_cols;
long matCsize = matC_rows * matC_cols;

int rank,size;
double start_time,end_time;

if(argc < 4)
{
printf("Not enough arguments\n");
}
else
{
matA_rows = atoi(argv[1]);
mat_comm_dim = atoi(argv[2]);
matA_cols = matB_rows = mat_comm_dim;
matB_cols = atoi(argv[3]);
matC_rows = matA_rows;
matC_cols = matB_cols;
matAsize = matA_rows * matA_cols;
matBsize = matB_rows * matB_cols;
matCsize = matC_rows * matC_cols;
}

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

int *matrixA,*matrixB,*matrixC;
matrixA = (int *)calloc(matAsize,sizeof(int));
matrixB = (int *)calloc(matBsize,sizeof(int));
matrixC = (int *)calloc(matCsize,sizeof(int));
int temp,counter1,counter2,counter3;

//TODO: read input matrices from file

//initialize matrix A with random numbers
for(counter1=0 ; counter1 < matA_rows ; counter1++)
{
 for(counter2=0; counter2 < matA_cols; counter2++) 
 { *(matrixA + counter1*matA_cols + counter2) = (int) random()%65536; }
}

//initialize matrix B with random numbers
for(counter1=0 ; counter1 < matB_rows ; counter1++)
{
 for(counter2=0; counter2 < matB_cols; counter2++) 
 { *(matrixB + counter1*matB_cols + counter2) = (int) random()%65536; }
}

start_time = MPI_Wtime();

//do matrix multiplication
for(counter1=0; counter1 < matC_rows; counter1++)
{
  for(counter2=0; counter2 < matC_cols; counter2++)
  {
    temp = 0;
    for(counter3=0; counter3 < mat_comm_dim; counter3++)
    { temp = temp + *(matrixA + counter1*matA_cols + counter3) * *(matrixB + counter3*matB_cols + counter2); }
    *(matrixC + counter1*matC_cols + counter2) = temp;
  }
}

end_time = MPI_Wtime();
printf("Walltime: %f\n",end_time - start_time);

//TODO: write results to file

//free allocated space
free(matrixA);
free(matrixB);
free(matrixC);
MPI_Finalize();
}
