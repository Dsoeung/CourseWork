#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SZ 32

static int total1;
static int total2;

static void mmm2(int *array, int size) {
    int sum = 0; 
    for (int i = 0; i < size; i++){  
    total1 += array[i];  
    total2 += array[i]*array[i];
    }
    return;
}

int main(int argc, char *argv[]) {
  int data[SZ];
  int i;
  for (i = 0; i < SZ; i++) data[i] = i % 10 + i / 10;
  mmm2(data, SZ);
  printf("Sum1=%d Sum2=%d\n", total1, total2);
  // Check md5sum: 0c24b26e66ad021e9f7e8952ab55c3d3
  return 0;
}
