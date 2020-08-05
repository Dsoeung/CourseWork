#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pagetable.h"

//Create page table intitializing all entries validity to 0
void initializePT(PageTable *PT, unsigned int entries) {
    int i;
    for (i = 0; i < entries; i++) {
        PT[i].vi = 0;
    }
}
