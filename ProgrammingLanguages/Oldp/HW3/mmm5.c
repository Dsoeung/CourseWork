#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SZ 65

typedef struct MMM {
  int total1;
  int total2;
} mmm_t;

static mmm_t *mmm5(int *array, int size) {
  mmm_t *mmm = (mmm_t *) malloc(sizeof(mmm_t));
  // TBD
  mmm->total1 = 0;
  mmm->total2 = 0;
  int i = 0;
  for (i = 0; i < size; i++){  
    mmm->total1 += array[i];  
    mmm->total2 += array[i]*array[i];
    }
  return mmm;
}

int main(int argc, char *argv[]) {
  int data[SZ];
  int i;
  for (i = 0; i < SZ; i++) data[i] = i % 10 + i / 10;
  mmm_t *mmm = mmm5(data, SZ);
  printf("Sum1=%d Sum2=%d\n", mmm->total1, mmm->total2);
  // Check md5sum: 90ce0f7083a8f701a92d3592fdb78b88
  // I'm strict on what I should or shouldn't do should I free mmm here?
  return 0;
}
