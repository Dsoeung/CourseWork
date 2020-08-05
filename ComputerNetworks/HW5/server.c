// Server side program to demonstrate Socket programming C
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

#define PORT 8008 //50009
#define MAXLINE 98

int main(int argc, char const *argv[])
{
	int i;
    int sock, new_sock, valread;
    struct sockaddr_in server, from;
    int set_opt = 1;
    int addrlen = sizeof(server);
    char buffer[MAXLINE] = {0};
    char *hello = "Hello from server";
    pid_t pid;
	int size;
	int nbytes;
	int flags;
   
    /* create a socket called sock. It is a stream socket */
	sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sock < 0) {
		perror("socket");
		return -1;
	}

    /* set SO_REUSEADDR in case main server needs to be restarted 
	while a child is running */
    i = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&set_opt, sizeof(set_opt));
	if( i < 0) {
		perror("setsockopt");
		return -1;
	}
	
	memset(&server, 0, sizeof(server)); 
    memset(&from, 0, sizeof(from));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );

  
    if (bind(sock, (struct sockaddr *)&server, sizeof(server))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
	
	i = listen(sock, 3);
	if( i < 0) {
		perror("listen");
		return -1;
	} else
	fprintf(stderr, "Simple TCP server process %d is ready!\n\n",(int)getpid());
    while (1){
		addrlen = sizeof(server);
					//accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<
		new_sock = accept(sock, (struct sockaddr *) &from, &addrlen);
		if( new_sock < 0) {
			if(errno == EINTR)
			continue; /*if interrupted just try again */
			else {
				perror("accept");
				return -1;
			}
		}
		pid = fork(); /* if pid == 0, I am the child process */
		if( pid == 0) {
			close(sock); /* child does not need original listening socket */
			nbytes = MAXLINE; /* receive packets up to 99 bytes long */
			flags = 0; /* must be zero or this will not work! */
			size = recv(new_sock, buffer, nbytes, flags);
			if(size < 0) {
				perror("recv");
				return -1;
			}
			printf("%s\n",buffer );
			sleep(1);
			send(new_sock , hello , strlen(hello) , 0 );
			printf("Hello message sent\n");
			close(new_sock);
			//d0
		}else { /* I am the parent process */
			if(pid == -1) {
				perror("fork");
				return -1; 
			}
			close(new_sock); /* parent does not need new_sock */
		}
	}
    return 0;
}