int handleNewReq(char * exename, char * argv[], struct init * data) {
	int fd[2];
	pid_t cpid;
	size_t structSize = sizeof(struct init);
 // Create the pipe that the parent will use to send the data
 // structure to the child process.
 if(pipe(fd) == -1) {
	 perror("pipe failed");
	 return EXIT_FAILURE;
	 }
 // Fork to create the child process.
 switch(cpid = fork()) {
	case -1:
	perror("fork failed");
	return EXIT_FAILURE;
 case 0: // child
 // Dup the read end of the pipe into the child's stdin.
 if(dup2(fd[0], STDIN_FILENO) == -1) {
	 perror("dup2 failed");
	 return EXIT_FAILURE;
	 }
 // Cleanup unneeded/duplicate file descriptors.
 close(fd[0]);
 close(fd[1]);
 // Execute the command passed into this function.
 execvp(exename, argv);
 perror("execvp failed");
 return EXIT_FAILURE;
default: // parent
 // Write the data structure to the child via the write
 // end of the pipe.
 if(write(fd[1], data, structSize) != structSize) {
	 perror("write failed");
	 return EXIT_FAILURE;
	 }
 // Cleanup unneeded/duplicate file descriptors.
 close(fd[0]);
 close(fd[1]);
 // Wait for the child to complete before returning to the
 // caller.
 waitpid(cpid, NULL, 0);
 }
 return 0;
}