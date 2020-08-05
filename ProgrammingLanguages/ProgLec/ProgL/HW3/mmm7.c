#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SZ 87

#define mmm7(ar, sz, t1, t2) \
  t1 = 0; \
  t2 = 0; \
  int j;\
  for (j = 0; j < sz; j++){ \
   t1 += ar[j];  \
   t2 += ar[j]*ar[j]; \
  }\
  
  
int main(int argc, char *argv[]) {
  int data[SZ];
  int i;
  int total1;
  int total2;
  for (i = 0; i < SZ; i++) data[i] = i % 10 + i / 10;
  mmm7(data, SZ, total1, total2);
  printf("Sum1=%d Sum2=%d\n", total1, total2);
  // Check md5sum: 4432d1643d3e91b41dee8ed0620d5d07
  return 0;
}