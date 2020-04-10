// your C program using sse instructions
#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <sys/time.h>
#include <time.h>

// Wall time function to get the time for some process
void get_walltime(double *wct)
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    *wct = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}

int main(){
  // initialization
  float *a, *b;
  float num[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  float K = 0.5;
  float K4 = 5.0;
  double time_taken, ts, te;
  __m128 *va, *vb, va0, va1, va2, va3, va4, va5, va6, va7, va8, sum0, sum1, sum2, sum3, sum4, sum5, sum6, sumF;
  int i;

  // Allocate memory for the arrays
  i = posix_memalign((void**)&a, 16, N*M*sizeof(float));
  if(i!=0){exit(1);}
  i = posix_memalign((void**)&b, 16, N*M*sizeof(float));
  if(i!=0){exit(1);}

  // Give some values
  for(i=0; i<N; i++){
    a[i] = i*2;
    b[i] = i;
  }

  // package
  get_walltime(&ts);
  for (i=1;i<N-1;i++) {
    va = (__m128*)a;
    vb = (__m128*)b;
    for (int j=1;j<M-1;j+=4) {

      // Put the needed values in to the num array and then loading to __m128 to process
      num[0] = a[M*i+j]*K4;
      va0 = _mm_load_ps(&num[0]);
      num[1] = a[(i-1)*M+(j-1)]*K;
      va1 = _mm_load_ps(&num[1]);
      num[2] = a[(i-1)*M+j]*K;
      va2 = _mm_load_ps(&num[2]);
      num[3] = a[(i-1)*M+(j+1)]*K;
      va3 = _mm_load_ps(&num[3]);
      num[4] = a[M*i+(j-1)]*K;
      va4 = _mm_load_ps(&num[4]);
      num[5] = a[M*i+(j+1)]*K;
      va5 = _mm_load_ps(&num[5]);
      num[6] = a[M*(i+1)+(j-1)]*K;
      va6 = _mm_load_ps(&num[6]);
      num[7] = a[M*(i+1)+j]*K;
      va7 = _mm_load_ps(&num[7]);
      num[8] = a[M*(i+1)+(j+1)]*K;
      va8 = _mm_load_ps(&num[8]);

      // Add every value we need for every pixel
      sum0 = _mm_add_ps(va0, va1);
      sum1 = _mm_add_ps(va2, va3);
      sum2 = _mm_add_ps(va4, va5);
      sum3 = _mm_add_ps(va6, va7);
      sum4 = _mm_add_ps(sum0, sum1);
      sum5 = _mm_add_ps(sum2, sum3);
      sum6 = _mm_add_ps(sum4, sum5);
      sumF = _mm_add_ps(sum6, va8);

      // final value
      *vb = sumF;
      // Get va and vb to the next cell
      va++;
      vb++;
    }
  }
  get_walltime(&te);

  // Tme taken
  time_taken = te - ts;

  // Results
  printf("%lf\n", time_taken);
  printf("Process: %lf\n", (2.0*N*M)/((time_taken)*1e6));

  free(a);
  free(b);

  return 0;
}
