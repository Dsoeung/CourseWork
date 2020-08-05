#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAXSIZE 4096

/****Guide Lines ****/
/*
 *Indent 3 spaces for each level and do not use tab characters.
 *Blocks: starting brace does not start a new line.
 *At most one statement per line.
 *At most one assignment per line.
 */

//Function invoked to count number of words inside of file fName.
int wCount(char *fName){
   char cCurr;
   char *pszBuffer;
   int i;
   int iWordCnt;
   FILE *fTemp;
   size_t szLine = MAXSIZE;   
   pszBuffer = calloc(szLine, sizeof(char));

   //Open fName for read
   fTemp = fopen(fName, "r");
	
   //Check if fopen succeeded
   if(fTemp == NULL) {
	   printf("ERROR: fopen failed, file could not be open : %s \n", fName);
       return -1;
   }
	
   i = 0;
   iWordCnt = 1;
   
   while(1){ 
      pszBuffer[i] = (char)getc(fTemp);
	  cCurr = pszBuffer[i];
	  if(cCurr == EOF) { //read til EOF and we can break from while!
         break;
      }
	  //For word counting, you could simply use the white-space characters~
	  //(e.g., ' ', '\t', '\n') as the delimiter.
	  if(cCurr == ' ' || cCurr == '\t' || cCurr == '\n') {
         iWordCnt++;
      }
	  //reached the end so we iterate on
         i++;
   }
	//free the pszBuffer !
    free(pszBuffer);
	
	//close fTemp file we opened to read.
    fclose(fTemp);
	
	//return the number of words in the file!
    return iWordCnt;
}

int main(int argc, char *argv[]) {
    char *pfileName;
	int iFiles = argc;
	int iWordCnt;
	int i;
    pid_t pid;
	
	//Error check number of arguments.
	/* ./wordcount File_1 File_2 ... File_n */
    if(argc < 2) {
        printf("ERROR: 1 or more input files needed. \n");
        printf("USAGE: ./wordcount File_1 File_2 ... File_n \n");
        return -1;
    }

    // Iterate over each file and fork a file process that counts the number of words in each file
    // The number of words is then printed and the child process exits
    for(i = 1; i < iFiles; i++) {
        pid = fork();
		
		//Error check fork, <0
        if(pid < 0) {
            printf("ERROR: Fork has failed ! \n");
        }
		
		//Child process
		//invoke a function to count the number of words inside a specific file and print out the results.
        if(pid == 0) { 
            pfileName = argv[i];
			//each child process individually invokes wCount
            iWordCnt = wCount(pfileName);
            printf("Child process for %s: Number of words is %d\n", pfileName, iWordCnt);
            return 0;
        }
    }
	
    while(1) {
		//The main process should wait until all child processes report their results;
        pid = wait(NULL);
        if((pid == -1)) {
            break;
        }
    }
	//the main process report the end of the program with the print out:
    printf("All %d files have been counted!\n", iFiles - 1);
    return 0;
}

