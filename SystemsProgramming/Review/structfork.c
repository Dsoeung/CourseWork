#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define INIT_PID 9999999

struct message {
 pid_t my_pid;
 pid_t recv_pid;
};

int main(int argc, char ** argv) {
 // Initialize cpid to be non-zero in order to allow the parent to
 // enter the for-loop below.
 pid_t cpid = INIT_PID;
 pid_t my_pid;
 pid_t their_pid;
 int my_index = 0;
 int num_fork;
 int i;
 struct message msg;
 size_t msg_size = sizeof(struct message);
 // The user should supply exact one command line argument.
 if(argc != 2) {
	fprintf(stderr, "Usage: %s <num>\n", argv[0]);
	return EXIT_FAILURE;
	}
	
 num_fork = atoi(argv[1]);
 // We need a pipe for every child, as well as the parent.
 int fd[num_fork + 1][2];
 // Create all of the necessary pipes.
 for(i = 0; i <= num_fork; i++) {
 if(pipe(fd[i]) == -1) {
 perror("pipefailed");
 return EXIT_FAILURE;
 }
 }
 // The parent will now create num_fork number of child processes.
 // The "cpid > 0" within the loop condition ensures that child
 // processes will exit the loop, as we do NOT want them to begin
 // participating (without this, we would end up with 2^num_fork
 // child processes).
 for(i = 1; i <= num_fork && cpid > 0; i++) {
 switch(cpid = fork()) {
 case -1:
 perror("fork failed");
 return EXIT_FAILURE;
 case 0: // child process
 // The child will simply update their index number to
// equal the current value of the loop counter.
 my_index = i;
 }
 }
 // All processes will obtain their PID value.
 my_pid = getpid();
 // No process will ever need to write to its own pipe, thus
 // we should close our own write end.
 close(fd[my_index][1]);
 if(my_index > 0) {
 // Children close all other child pipes' read ends,
 // as a child will only ever read from its own pipe.
 for(i = 0; i <= num_fork; i++) {
 if(my_index != i) {
 close(fd[i][0]);
 }
 }
 } else {
 // Parent closes both ends of ALL child pipes, as it
 // will never have to read OR write on any of their
 // pipes (it will only read from its own pipe).
 for(i = 1; i <= num_fork; i++) {
 close(fd[i][0]);
 close(fd[i][1]);
 }
 }
 // Only child processes will enter this block...
 if(my_index > 0) {
 // Only odd-PID children will write to other children
 if(my_pid % 2 == 1) {
 // Loop through all child process' indexes
 // (i.e., from 1 <= i <= num_fork).
 for(i = 1; i <= num_fork; i++) {
 // Make sure I do not write a message to myself...
 if(i != my_index) {
 // Now write my PID to this sibling processs...
 if(write(fd[i][1], &my_pid, sizeof(pid_t)) !=
 sizeof(pid_t)) {
 perror("write failed");
 return EXIT_FAILURE;
 }
 }
 }
 }
 // ALL children should now clean up their pipe ends (close write
 // ends of all other children)
 for(i = 1; i <= num_fork; i++) {
 if(i != my_index) {
 close(fd[i][1]);
 }
 }
 }
 // All children will now read their pipes for the PIDs they
 // received from their siblings
 if(my_index > 0) {
 while(read(fd[my_index][0], &their_pid, sizeof(pid_t)) ==
 sizeof(pid_t)) {
 // Build the message I will send to my parent by populating
 // this data structure with my PID as well as the sibling who
 // wrote to my pipe.
 msg.my_pid = my_pid;
 msg.recv_pid = their_pid;
 // Send the data structure to my parent via their pipe.
 if(write(fd[0][1], &msg, msg_size) != msg_size) {
 perror("write failed");
 return EXIT_FAILURE;
 }
 }
 // Each child will now close their own pipe's read end (as they
 // are done reading), as well as the write end of the parent's
 // pipe (as they are also done writing).
 close(fd[my_index][0]);
 close(fd[0][1]);
 } else { // parent will now read its pipe for messages
 while(read(fd[0][0], &msg, msg_size) == msg_size) {
 printf("pid %d received data from pid %d\n", msg.my_pid,
 msg.recv_pid);
 }
 // Finally, the parent will close it's own pipe's read end, as
 // the pipe is now empty (we previously closed the write end, so
 // we don't need to worry about doing that).
 close(fd[0][0]);
 }
 return EXIT_SUCCESS;
}