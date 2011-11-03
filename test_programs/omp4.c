#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main()
{
int thread_id=0,numthreads=1,counter,sum;
long numelements = 160000000,allocation;
int finalsum=0;
int *array;
int *sumarray;

#pragma omp parallel private(thread_id,counter,sum)
{
thread_id = omp_get_thread_num();
if(thread_id==0)
{
numthreads = omp_get_num_threads();
array = calloc(numelements,sizeof(int));
sumarray = calloc(numthreads,sizeof(int));
for(counter=0;counter<numelements;counter++)
{ *(array+counter) =(int)  random()%32; }
allocation = numelements/numthreads;
}
#pragma omp barrier
sum=0;
for(counter=0;counter<allocation;counter++)
{ sum += *(array + thread_id*allocation + counter); }
*(sumarray + thread_id) = sum;
printf("I am thread %d. My sum is %d\n",thread_id,sum);
#pragma omp barrier
if(thread_id==0)
{
for(counter=0;counter<numthreads;counter++)
{ finalsum += *(sumarray + counter); }
printf("Sum of all numbers is %d\n",finalsum);
}
}

free(array);
free(sumarray);

return 0;
}
