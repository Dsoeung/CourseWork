//Darin Soeung
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TARGET_PORT 52025
#define MAXLINE 98

 int main(int argc, char *argv[]){
	int sock;
	int n;
	int i;
	int	len;
	int flags;
	char buffer[MAXLINE];
	char input [MAXLINE];
	//"67.11.114.92"
	char *str_addr = "67.11.114.92";
	struct sockaddr_in     servaddr; 
	struct sockaddr_in     me;
	
	//str_addr = 67.11.114.92
	servaddr.sin_addr.s_addr = inet_addr(&str_addr[0]);
	 
	 /* create a socket to send on */
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error("Socket error");
	}   
	
	memset(&servaddr, 0, sizeof(servaddr));
	
	printf("Hello from Client, enter your string: \n");
	fgets(input, MAXLINE, stdin);
	input[strlen(input) -1] = '\0';

	

	// Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(TARGET_PORT); //8080
    servaddr.sin_addr.s_addr = inet_addr(str_addr);	//127.0.0.1
	
	me.sin_family = AF_INET;
	me.sin_port = htons(0);
	me.sin_addr.s_addr = INADDR_ANY;
	
	i = bind(sock, (struct sockaddr *) &me, sizeof(me));
	if( i < 0) {
	printf("bind result: %d\n", i);
	return -1;
	}
	
	flags = 0;
	sendto(sock, (const char *)input, strlen(input), 
        flags, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    printf("Message sent.\n"); 
	
	n = recvfrom(sock, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len);
	buffer[n] = '\0'; 
    printf("Server : %s\n", buffer);
	
	close(sock); 
    return 0; 
 }