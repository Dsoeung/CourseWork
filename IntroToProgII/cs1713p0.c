/**************************************************************************
cs1713p0.c by Darin Soeung
Purpose:
    This program computes the sum of a sequence of numbers
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    FILE *pfile;            // FILE variable
    int iNum;               // variable to store current number
    int iTotal;             // variable to store total value

    //check number of command line arguements
    if (argc !=2) {
        fprintf(stderr, "Usage: %s <dataFileName> \n", argv[0]);
        exit(-1);
    }
    // open the data file
    if ( (pfile =fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "open %s error \n", argv[1]);
        exit(-1);
    }
    // read one data at a time and sum all numbers
    iTotal = 0;
    while ( fscanf (pfile, "%d", &iNum) == 1){
        iTotal += iNum;
    }
    // print out the sum value to stdout
    printf("Sum = :%d\n", iTotal);
    
    return 0;
}
