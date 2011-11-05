#include <stdio.h>
#include <stdlib.h>

int main()
{
int *matrix = calloc(32000*32000,sizeof(int));
int counter1,counter2;

for(counter1=0;counter1<32000;counter1++)
{
for(counter2=0;counter2<32000;counter2++)
{ *(matrix + counter1*32000 + counter2) = (int) random(); }
}

return 0;
}
