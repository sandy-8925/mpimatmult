#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

//parallel matrix multiplication

int main()
{
int thread_id=0,nthreads,counter1,counter2,counter3,tempsum;
int matA_rows=1000,matA_cols=1000;
int matB_rows=1000,matB_cols=1000;
int matC_rows=matA_rows,matC_cols=matB_cols;
int *matrixA,*matrixB,*matrixC;
int *temp_ptr,*mstart,*mend,*ptr;

matrixA = calloc(matA_rows*matA_cols,sizeof(int));
matrixB = calloc(matB_rows*matB_cols,sizeof(int));
matrixC = calloc(matC_rows*matC_cols,sizeof(int));

//fill matrices A and B with random data
temp_ptr = mstart = matrixA;
mend = mstart + matA_rows*matA_cols;

while(temp_ptr < mend)
{ *(temp_ptr++)=(int) random(); }

temp_ptr = mstart = matrixB;
mend = mstart + matB_rows*matB_cols;

while(temp_ptr < mend)
{ *(temp_ptr++)=(int) random(); }

#pragma omp parallel private(thread_id,counter1,counter2,counter3,tempsum)
{
thread_id = omp_get_thread_num();
if(thread_id==0)
{ nthreads = omp_get_num_threads(); }
#pragma omp barrier
//divide rows of matrixA evenly among all threads and do matrix multiplication in parallel
for(counter1 = thread_id*matC_rows/nthreads ; counter1 < ((thread_id+1)*matC_rows)/nthreads ; counter1++ )
{
for(counter2 = 0 ; counter2 < matC_cols ; counter2++)
{
tempsum = 0;
for(counter3 = 0 ; counter3 < matB_rows ; counter3++ )
{ tempsum += *(matrixA + counter1*matA_cols + counter3  ) * *(matrixB + counter3*matB_cols + counter2 ) ; }
}
}
}

return 0;
}

