#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "phypages.h"

//return index of 

int findFreeFrames(unsigned int *freeframes , unsigned int frames) {
    int i;
	
    for(i = 1; i <= frames; i++) {
        if (freeframes[i] == 1) {
            freeframes[i] = 0;
            return i;   // if free frame is found, return i
        }
    }
    return 0;   // hit this return if no free frames are found.
}


int getVictimLRU(unsigned int *LRUCount, unsigned int frames) {
    int i = 1, frame = 1;  // frame variable to hold return value.
    int min = LRUCount[1];  // initialize the minimum to the first value in the array.
    for (i = 1; i <= frames; i++) {
        if (min > LRUCount[i]) {   
            min = LRUCount[i];   // update min
            frame = i;  // save the frame that is to be returned upon exiting the loop.
        }
    }
    return frame;
}
