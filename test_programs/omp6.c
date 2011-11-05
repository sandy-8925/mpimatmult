#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
int thread_id=0,numthreads,counter,nels;
if(argc>1)
{ nels = atoi(argv[1]); }
int *matrix = calloc(nels,sizeof(int));

#pragma omp parallel private(thread_id)
{
thread_id = omp_get_thread_num();
if(thread_id == 0)
{ numthreads = omp_get_num_threads(); }
#pragma omp barrier
#pragma omp for nowait
for(counter=0;counter<nels;counter++)
{ *(matrix + counter) = (int) random(); }
}

return 0;
}
