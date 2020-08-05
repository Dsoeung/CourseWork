#include<stdio.h>
#include<stdlib.h>
#include <unistd.h> // import library

//is (ACAbac) possible?

int main() 
{
    printf("A");  //parent 1 step1
    if(fork() == 0) {
        if(fork()) 
        {
            printf("b"); //child step 3
        } else {
            printf("c"); //grand exit child
            exit(0);
        }
        printf("a");    //child 2 step 2 repeated step 4
    } 
    printf("C");    //parent 1 step 1
    return 0;
}

//prints parent complete, always start, AC
//prints parent pt.1(A) then child variants pt.2 (C), AbaC
//repeat
//print parent pt.1 (A)
//find exit(0), print c -exit, always end, c

//this program
//AC parent is always printed
//fork makes 2 parent process go through either
//Ac
//or AbaC
//only 2 variants of output are
//ACAcAbaC
//or
//ACAbaCAc