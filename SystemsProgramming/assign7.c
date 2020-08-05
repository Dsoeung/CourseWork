#include <stdio.h>
int main(){

typedef struct
 {
 char firstName [32];
 char lastName [32];
 char leaseStart [16];
 char leaseEnd [16];
 int balance ;
 } Apartment ;
 
printf("Enter one of the following actions or press CTRL-D to exit.\n");
printf("C - create a new apartment record\n");
printf("R - read an existing apartment record\n");
printf("U - update an existing apartment record\n");
printf("D - delete an existing apartment record\n");

gets("Enter a Letter", %2s );



 
 }