#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	unsigned long LA, PA; 
	unsigned int PT[32] = {2, 4 ,1, 7, 3 ,5 ,6};
	unsigned int p = 5, f = 3, d = 7;
 	unsigned int pnum, fnum, dnum;
	int inFile;
	int output;
	
	if(argc != 2){
		perror("ERROR USAGE: <./name> <inputFile>\n");
		exit(-1);
	}
    
    if((inFile = open(argv[1], O_RDONLY)) == -1){
		perror("ERROR Could not open file");
		exit(-1);
    }
	
	//them in a file called part1-output
    if((output = open("part1-output", O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1){
		perror("ERROR Could not write to output file");
		exit(-1);
    }
	
    while(read(inFile, &LA, sizeof(unsigned long)) == sizeof(unsigned long)){
		pnum = LA >> d;
		fnum = PT[pnum];
		dnum = LA & ((1 << d)-1);//2^dnum -1 
		PA = (fnum << d) + dnum;
		printf("Physical Address = 0x%lx\n",PA);
		//write to output file 
    	write(output, &PA, sizeof(unsigned long));
	}
}