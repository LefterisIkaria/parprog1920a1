// your C program without sse instructions
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>


// Wall time function to get the time for some process
void get_walltime(double *wct)
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    *wct = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}

int main()
{
    // Initialization
    float *a, *b;
    float K = 0.5;
    float K4 = 5.0;
    double ts, te;
    double time_taken;

    // Allocate memory to create array a, b
    a = (float*) malloc(N * M * sizeof(float));
    b = (float*) malloc(N * M * sizeof(float));

    // Check if the memory allocation was fine
    if (a == NULL || b == NULL)
    {
       printf("Error! memory not allocated.\n");
       exit(0);
    }

    //Giving some values in the arrays
    for(int i = 0; i < N*M; i++)
    {
      a[i] = i*2;
      b[i] = i;
    }

    // Start of the package
    get_walltime(&ts);
    for(int i = 1; i < N-1; i++){     //rows
      for(int j = 1; j < M-1; j++){   //cols
          b[j*N+i] = a[M*i+j]*K4 + a[(i-1)*M+(j-1)]*K + a[(i-1)*M+j]*K + a[(i-1)*M+(j+1)]*K + a[M*i+(j-1)]*K+ a[M*i+(j+1)]*K + a[M*(i+1)+(j-1)]*K + a[M*(i+1)+j]*K+ a[M*(i+1)+(j+1)]*K;
      }
    }
    get_walltime(&te);

    // The total time of the process
    time_taken = te - ts;

    // Time
    printf("Time:     %lf\n", time_taken);
    // MegaProspelaseis/sec(megaFlops/sec)
    printf("Process: %lf\n", (2.0*N*M)/((time_taken)*1e6));

    // Free the arrays
    free(a);
    free(b);

    return 0;
}

