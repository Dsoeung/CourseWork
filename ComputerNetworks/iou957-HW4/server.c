//Darin Soeung
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT     8080 
#define MAXLINE 98 
  
// Driver code 
int main(int argc, char *argv[]) { 
    int sock; 
	int len;
	int size;
	int i;
    char buffer[MAXLINE]; 
    char *respond = "Server connection complete"; 
    struct sockaddr_in server, from; 
      
    if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&server, 0, sizeof(server)); 
    memset(&from, 0, sizeof(from)); 
      
    server.sin_family    = AF_INET; 
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons(PORT); 
      
    i = bind(sock, (struct sockaddr *) &server, sizeof(server));
	if( i < 0) {
	printf("bind result: %d\n", i);
	return -1;
	} else
	printf("Simple UDP server is ready!\n\n");  
  
    len = sizeof(from);
  
    size = recvfrom(sock, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &from, 
                &len); 
    buffer[size] = '\0'; 
    printf("Client : %s\n", buffer); 
    sendto(sock, (const char *)respond, strlen(respond),  
        MSG_CONFIRM, (const struct sockaddr *) &from, 
            len); 
    printf("Return message sent.\n");  
      
    return 0; 
} 

