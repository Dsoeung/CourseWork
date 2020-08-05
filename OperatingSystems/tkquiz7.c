#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
int main (int argc, char *argv[]) {
	int Pipe1[2];
	int Pipe2[2];
	int ch1pid, ch2pid, numread, numwrite;
	char buf;
	
	if(pipe(Pipe1)==-1){
		perror("failed to create pipe 1");
		return 1;
	}
	
	if((ch1pid = fork())==-1){
	    perror("failed to create child");
	    return 1;
	}
	
	if(ch1pid == 0){ /* child 1 */
	dup2(Pipe1[1], STDOUT_FILENO);
	close(Pipe1[0]);
	close(Pipe1[1]);
	fprintf(stderr,"Child 1 is ready...");
	execl(argv[1],"prog1",NULL);
	perror("failed to exec prog1");
	return 1;
	}

	
	if(pipe(Pipe2)==-1){
		perror("failed to create pipe 1");
		return 1;
	}
	if((ch2pid = fork())==-1){
	    perror("failed to create child");
	    return 1;
	}
	
	if(ch2pid == 0){ /* child 2 */
	dup2(Pipe2[0], STDIN_FILENO);
	close(Pipe2[0]);
	close(Pipe2[1]);
	fprintf(stderr,"Child 2 is ready...");
	execl(argv[2],"prog2",NULL);
	perror("failed to exec prog2");
	return 1;	
	}
	
	//Parent
	dup2(Pipe1[0], STDIN_FILENO);
	close(Pipe1[1]);
	close(Pipe1[0]);
	dup2(Pipe2[1], STDOUT_FILENO);
	close(Pipe2[1]);
	close(Pipe2[0]);
	
		while (1){
			numread = read(STDIN_FILENO,&buf, 1);
			if (numread <= 0) break;
			numwrite = write(STDOUT_FILENO,&buf, 1);
			if (numwrite <= 0) break;
		}
	close(STDOUT_FILENO);
	waitpid(ch1pid);
	waitpid(ch2pid);
	return 1;
}