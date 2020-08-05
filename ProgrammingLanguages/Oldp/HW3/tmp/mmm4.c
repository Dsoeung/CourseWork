#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SZ 54

typedef struct MMM {
  int total1;
  int total2;
} mmm_t;

static void mmm4(int *array, int size, mmm_t *mmm) {
  // TBD
    mmm->total1 = 0;
    mmm->total2 = 0;
  for (int i = 0; i < size; i++){  
    mmm->total1 += array[i];  
    mmm->total2 += array[i]*array[i];
    }
    return;
}

int main(int argc, char *argv[]) {
  int data[SZ];
  int i;
  mmm_t mmm;
  for (i = 0; i < SZ; i++) data[i] = i % 10 + i / 10;
  mmm4(data, SZ, &mmm);
  printf("Sum1=%d Sum2=%d\n", mmm.total1, mmm.total2);
  // Check md5sum: 2a4836661d821ab46e435e4ac3397681
  return 0;
}
