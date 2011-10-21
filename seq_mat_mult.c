#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define mat_comm_dim 1000
#define matA_rows 1000
#define matA_cols mat_comm_dim
#define matB_rows mat_comm_dim
#define matB_cols 1000
#define matC_rows matA_rows
#define matC_cols matB_cols


int main()
{
int *matrixA,*matrixB,*matrixC;
matrixA = (int *)calloc(sizeof(int),matA_rows*matA_cols);
matrixB = (int *)calloc(sizeof(int),matB_rows*matB_cols);
matrixC = (int *)calloc(sizeof(int),matC_rows*matC_cols);
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

//TODO: write results to file

//free allocated space
free(matrixA);
free(matrixB);
free(matrixC);

}
