#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main()
{
int th_id=0, nthreads=1,counter;
int *array;

#pragma omp parallel private(th_id)
{
if(th_id==0)
{
nthreads = omp_get_num_threads();
array = calloc(nthreads,sizeof(int));
for(counter=0;counter<nthreads;counter++)
{ *(array + counter) = (int) random(); }
}
#pragma omp barrier
th_id = omp_get_thread_num();
printf("Hello world from thread %d. My random number is: %d\n",th_id,*(array+th_id));
#pragma omp barrier
if(th_id==0)
{
nthreads = omp_get_num_threads();
printf("There are %d threads\n",nthreads);
}
}
return 0;
}
