#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "pagetable.h"
#include "phypages.h"

int main (int argc, char *argv[]) {
	int e, y;
	int x = 0;										
    PageTable PT[32];                              	//using new PT struct
	unsigned int p = 5, f = 3, d = 7;              	//same locals as p1    
	unsigned int pnum, dnum, fnum;                     
	unsigned int CLK = 0;                           
    unsigned int freeframes[8] = {0,1,1,1,1,1,1,1};    
    unsigned int LRUCount[8] = {0};                    
    unsigned int revmap[8] = {-1};        		   //Not pointing to any pages yet       
	unsigned int pFaults = 0;                       
    unsigned long PA, LA;
	int inFile;   
    int output;  
	
	initializePT(PT,32);
    
    if((inFile = open(argv[1], O_RDONLY)) == -1){
		perror("ERROR Could not open file");
		exit(-1);
    } 
	
	//them in a file called part2-output
    if((output = open("part2-output", O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1){
		perror("Unable to write to file");
		exit(-1);
    }

    while (read(inFile, &LA, sizeof(unsigned long)) == sizeof(unsigned long)) {
         pnum = LA >> d;      		
         dnum = LA & ((1 << d)-1);
         CLK++;    					//iterate
         if (PT[pnum].vi == 1) {
			 fnum = PT[pnum].fnum;
             PA = (fnum << d) + dnum;   //same as part1
             revmap[fnum] = pnum; 
             LRUCount[fnum] = CLK; 
             write(output, &PA, sizeof(unsigned long));   
         } else {
             pFaults++;  
             x = findFreeFrames(freeframes, d);
             if (x > 0) {             
                 PT[pnum].fnum = x;
                 PT[pnum].vi = 1;
				 fnum = PT[pnum].fnum;
                 PA = (fnum << d) + dnum;    //same as part1
                 revmap[fnum] = pnum;  
                 LRUCount[fnum] = CLK;    
                 write(output, &PA, sizeof(unsigned long));   
             } else {         //LRU        
                int y = getVictimLRU(LRUCount, d);  
                PT[revmap[y]].vi = 0;   
                PT[pnum].fnum = y;      
                PT[pnum].vi = 1;           
				fnum = PT[pnum].fnum;
                PA = (fnum << d) + dnum;      //same as part1
                write(output, &PA, sizeof(unsigned long)); 
                LRUCount[fnum] = CLK;
                revmap[fnum] = pnum;
             }
         }
    }
	
    printf("Part 2 page faults: %d\n", pFaults);
    return 0;
}