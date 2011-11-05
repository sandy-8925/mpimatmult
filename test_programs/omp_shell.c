#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main()
{
int thread_id=0,numthreads;

#pragma omp parallel private(thread_id)
{
thread_id = omp_get_thread_num();
if(thread_id == 0)
{ numthreads = omp_get_num_threads(); }
#pragma omp barrier
}

return 0;
}
