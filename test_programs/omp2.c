#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main()
{
int th_id=0, nthreads=1;

#pragma omp parallel private(th_id)
{
th_id = omp_get_thread_num();
printf("Hello world from thread %d. My random number is: %d\n",th_id,(int) random());
#pragma omp barrier
if(th_id==0)
{
nthreads = omp_get_num_threads();
printf("There are %d threads\n",nthreads);
}
}
return 0;
}
