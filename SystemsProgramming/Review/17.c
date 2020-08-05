#include<stdio.h>
#include<stdlib.h>
#include <unistd.h> // import library

int main() {
	
	if(fork() == 0){ 
		
	printf("a"); //if child ex
	
	if(fork()){ 
		
		printf("b"); //
		
		}else{
			
		exit(0);
		
		}
		
	}
	
	printf("c"); 
	
	return 0;
}