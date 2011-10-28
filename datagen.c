#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>



int main()
{
//TODO: accept these variables as commandline arguments

int mat_comm_dim = 1000;
int matA_rows = 1000,matA_cols = mat_comm_dim,matB_rows = mat_comm_dim,matB_cols = 1000;
char *filename="testdata"; //TODO: accept as commandline argument

int *matrixA,*matrixB;
int counter1,counter2;
int num_matrices=2,outfildesc;

matrixA = (int *)calloc(sizeof(int),matA_rows*matA_cols);
matrixB = (int *)calloc(sizeof(int),matB_rows*matB_cols);

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

//TODO: add error checking to all file operations
outfildesc = open(filename, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR); 

write(outfildesc, &num_matrices, sizeof(num_matrices));
write(outfildesc, &matA_rows, sizeof(matA_rows));
write(outfildesc, &matA_cols, sizeof(matA_cols));
write(outfildesc, &matB_rows, sizeof(matB_rows));
write(outfildesc, &matB_cols, sizeof(matB_cols));
write(outfildesc, matrixA, matA_rows*matA_cols*sizeof(int));
write(outfildesc, matrixB, matB_rows*matB_cols*sizeof(int));

close(outfildesc);

return 0;
}
