// Client side program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#define PORT 51111 //50009
#define MAXLINE 98

int main(int argc, char const *argv[])
{
	
    struct sockaddr_in address;
	int i;
    int sock = 0;
	int valread;
	int nbytes;
	int flags;
	int ssize;
	int rsize;
    struct sockaddr_in serv_addr;
	struct sockaddr_in me;
    char input [MAXLINE];
    char buffer[MAXLINE] = {0};
	char *str_addr = "67.11.114.92"; //"67.11.114.92"
	
	sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (sock < 0)
    {
        printf("\n Socket Error = %d errno = %d \n", sock, errno);
        return -1;
    }
	
	printf("Hello from Client, enter your string: \n");
	fgets(input, MAXLINE, stdin);
	input[strlen(input)-1] = '\0';
	
	memset(&serv_addr, '0', sizeof(serv_addr));
	
	//target_pc
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
	
	
	me.sin_family = AF_INET;
	me.sin_port = htons(0);
	me.sin_addr.s_addr = htonl(INADDR_ANY);
	
	i = bind(sock, (struct sockaddr *) &me, sizeof(me));
	if( i < 0) {
		printf("bind error = %d errno = %d\n", i, errno);
		return -1;
	}
	
/* 	
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    } */
	
	//if((i = connect(sock,(struct sockaddr *) &target_pc,sizeof(target_pc))) < 0) 
		
	serv_addr.sin_addr.s_addr = inet_addr(str_addr);
	
    if((i = connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr))) < 0) {
	printf("Connection attempt failed! Sure the address and port are right?\nCheck your firewall settings\n");
	printf("connect error = %d errno = %d\n", i, errno);
	return -1;
	}
	
	//Send message to server
	nbytes = strlen(input);
	flags = 0;
	/* we do not reference sockaddr_in since the socket is connected */
	ssize = send(sock, input, nbytes, flags);
	if(ssize < 0) { /* some kind of failure */
		printf("Send failure - errno = %d\n", errno);
		close(sock);
		return -1;
	}
	//recieve reply from sender
	nbytes = 256;
	rsize = recv(sock, buffer, nbytes, flags);
	if(rsize < 0) { /* some kind of failure */
		printf("Recv failure - errno = %d\n", errno);
		close(sock);
		return -1;
	}
	//server message
	printf("%s\n", buffer);
	close(sock); /* done with the connection */
    return 0;
}




























