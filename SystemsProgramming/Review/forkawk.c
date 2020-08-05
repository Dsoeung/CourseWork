//awk -f getuid.awk < /etc/passwd | sort -r

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

// example: awk -f getuid.awk < /etc/passwd | sort -r

int main(int argc, char ** argv) {
	int fd[2];
	int infilefd;
	// Create the pipe we will use to pass data from awk to sort.
	if(pipe(fd) == -1) {
		perror("pipe creation failed");
		return EXIT_FAILURE;
		}
 // Create a child process, which we will use to run "awk"
 // (the parent will be used to run "sort").
 switch(fork()) {
	 case -1:
	 perror("fork failed");
	 return EXIT_FAILURE;
	 
	 case 0: // child (awk)
 // Open the input file for read-only access.
 if((infilefd = open("/etc/passwd", O_RDONLY)) == -1) {
	 perror("open failed");
	 return EXIT_FAILURE;
	 }
 // Dup the input file's fd to standard input.
 if(dup2(infilefd, STDIN_FILENO) == -1) {
	 perror("dup2 failed");
	 return EXIT_FAILURE;
	 }
 // Dup the pipe's write end to standard output.
 if(dup2(fd[1], STDOUT_FILENO) == -1) {
	 perror("dup2 failed");
	 return EXIT_FAILURE;
	 }
 // Cleanup unneeded/duplicate file descriptors.
 close(infilefd);
 close(fd[0]);
 close(fd[1]);
 // Execute: awk -f getuid.awk
 execlp("awk", "awk", "-f", "getuid.awk", NULL);
 perror("execlp failed");
 return EXIT_FAILURE;
 
default: // parent (sort; the shell will wait on this process)
 // Dup the pipe's read end to standard input.
 dup2(fd[0], STDIN_FILENO);
 // Cleanup unneeded/duplicate file descriptors.
 close(fd[0]);
 close(fd[1]);
 // Execute: sort -r
 execlp("sort", "sort", "-r", NULL);
 perror("execlp failed");
 return EXIT_FAILURE;
 }
 // It would be impossible for us to read this line of code:
 // if the fork failed, we would have exited immediately. If either
 // execlp failed, we likewise would have returned immediately. Thus,
 // there is no actual path to this return statement.
 return EXIT_FAILURE;
}