#include<stdio.h>
#include<stdlib.h>
#include <unistd.h> // import library

int main() 
{
    printf("A");  //every or only parent
	
	
    if(fork() == 0) { //IF CHILD
        if(fork()) //IF CHILD/Parent of grandchild
        {
            printf("b"); 
        } else {
            printf("c"); //GrandChild
            exit(0);
        }
        printf("a"); //end if CHILD/Parent of grandchild
    } 
	
	
    printf("C");    //every or only parent
    return 0;
}

//output possibilities
//Parent 