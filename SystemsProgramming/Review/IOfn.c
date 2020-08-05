#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
//DEFINE MAX/BUF_SIZE 

#define BUF_SIZE 64

int main(int argc, char ** argv) {
//Accept 2 files in command args. using LL IO, print contents of first file to stdout
//followed by the contents of the second file to stdout also.
 int fdin1, fdin2;
 char * inFilename1, * inFilename2;
 char * buffer[BUF_SIZE];
 ssize_t bytesRead;
 // The user must supply exactly two command line arguments.
 if(argc != 3) {
	 fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
	 return EXIT_FAILURE;
	 }
	 inFilename1 = argv[1];	//file name initialization
	 inFilename2 = argv[2]; //file name initialization
 // Open both files for read-only access.
 if((fdin1 = open(inFilename1, O_RDONLY)) == -1) {
	 perror("open file failed");
	 return EXIT_FAILURE;
	 }
 // THESE INITIALIZE THE FILES TO BE READ ONLY, AND OPEN THEM 
 // INITIALIZE THEIR FILE DESCRIPTORS FDIN1 AND 2
 if((fdin2 = open(inFilename2, O_RDONLY)) == -1) {
	 perror("open file failed");
	 return EXIT_FAILURE;
	 }
 // Read the first file in a loop, fetching BUF_SIZE number of
 // bytes each time. Unless the file's size is a multiple of
 // BUF_SIZE, the last read operation will read < BUF_SIZE bytes.
 // This is why the call to write() specifies bytesRead number of
 // bytes, rather than BUF_SIZE.
 while((bytesRead = read(fdin1, buffer, BUF_SIZE)) > 0) {
	 if(write(STDOUT_FILENO, buffer, bytesRead) != bytesRead) {
		 perror("write failed");
		 return EXIT_FAILURE;
		 }
	}
	close(fdin1);
 // Repeat for the second file.
 while((bytesRead = read(fdin2, buffer, BUF_SIZE)) > 0) {
	 if(write(STDOUT_FILENO, buffer, bytesRead) != bytesRead) {
		 perror("write failed");
		 return EXIT_FAILURE;
		}
	}
		close(fdin2);
		return EXIT_SUCCESS;
}