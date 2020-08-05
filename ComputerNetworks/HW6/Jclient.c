// Jason Kha zki628 Assignment 6 CS 3873
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#define BUFFER_SIZE 40

void error(char *msg) {
    perror(msg);
    exit(0);
}

void sig_child(int signo) {
    pid_t pid;
    int stat;
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0);
    return;
}

struct data {
    double dif;
    double sum;
    int count;
};

int main(int argc, char *argv[]) {
    // declare server variables
    int i, socketfd, n, retries, drop;
    double dif, sum, dropPercent;
    char buffer[BUFFER_SIZE];
    struct data p;
    struct sockaddr_in server;
    struct timeval tv;
    unsigned long long start, end;
    socklen_t serverSize;

    // fd
    int fd[2];

    if(pipe(fd) < 0) {
        error("pipe");
    }

    if(fcntl(fd[0], F_SETFL, O_NONBLOCK) < 0) {
        error("fcntl");
    }

    // check for arguments
    if(argc != 3) {
        printf("Usage: ./client <port> <ip addres: xxx.yyy.zzz.nnn\n");
        exit(0);
    }

    // create a socket
    if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        error("Socket error");
    }

    // clear structure
    memset(&server, '\0', sizeof(server));

    // server info
    server.sin_family = AF_INET; // IPv4
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = inet_addr(argv[2]);

    serverSize = sizeof(server);

    // create child process
    int flags, nbytes, size, new_sock, num_received;
    pid_t pid, wpid;
    int status = 0;
    pid = fork();

    // if child process
    if(pid == 0) {
        nbytes = 99;
        flags = 0;
        num_received = 0;
        sum = 0;
        struct data c;
        // wait for response from server
        while(1) {
            // recvfrom
            if((n = recvfrom(socketfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &server, &serverSize)) < 0) {
                error("recv error");
            } else {
                // get time now
                gettimeofday(&tv, NULL);
                end = (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;
                printf("recvfrom - Message Received Time: %ld\n\n", (long) end); 

                // compute delta time
                dif = (long) end - atof(buffer);
                c.dif = dif;
                printf("recvfrom - Difference: %.0fms\n", dif);

                // accumulate total delta time
                sum += dif;
                c.sum = sum;
                printf("recvfrom - Sum: %.0fms\n", sum);

                // increment num_received
                num_received++;
                c.count = num_received;
                printf("recvfrom - num_received: %d\n", num_received);

                printf("recvfrom - Average RTT: %.0fms\n", sum / num_received);

                // send data to parent
                dup(fd[1]);
                write(fd[1], &c, sizeof(c));
            }
        }
    } else {
        // parent process
        if(pid == -1) {
            error("fork error");
        } else {

            // get specified number of messages
            printf("Enter specified number of messages: ");
            scanf("%d", &retries);
            for(i = 0; i < retries; i++) {
                // get time now
                gettimeofday(&tv, NULL);
                start = (unsigned long long) (tv.tv_sec) * 1000 + (unsigned long long) (tv.tv_usec) / 1000;
                
                //printf("%ld", (long) start);
                printf("--------------------------------------\n");
                // put time in message
			    memset(buffer, 0, BUFFER_SIZE);
			    sprintf(buffer, "%ld", (long) start);

                // send time to server
			    if((n = sendto(socketfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &server, sizeof(server))) < 0) {
				    error("sendto");
			    }
                printf("sendto   - Message Sent Time: %s\n", buffer);
                
                // get child process data
                dup(fd[0]);
                read(fd[0], &p, sizeof(p));

                // wait two seconds before sending another buffer
                usleep(2000000);
            }
            printf("--------------------------------------\n");
            
            // read last child process data
            read(fd[0], &p, sizeof(p));

            printf("Sum: %.2fms\n", p.sum);
            printf("Counts: %d\n", p.count);
            printf("Average RTT: %.2fms (%.0f/%d)\n", p.sum/p.count, p.sum, p.count);
            printf("Loss: %.2f%% (%d/%d)\n", 100.00 - ((double) p.count / (double) retries) * 100, p.count, retries);
            
            // kill child process
            kill (pid, SIGKILL);
        }
    }

    // wait for child process to finish
    // while ((wpid = wait(&status)) > 0);

    // close the socket
    close(socketfd);
}

