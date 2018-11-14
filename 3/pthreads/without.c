#include <stdio.h>
#include <time.h>


int main() {
  long int i=0;
  unsigned long sum = 0;
  clock_t tic = clock();
  while(i<10000000) {
    //printf("%ld\n",i);
    sum += i;
    //printf("%lu\n", sum);
    i++;
  }
  clock_t toc = clock();
  printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
  printf("%lu\n", sum);
}
