//Darin Soeung
#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define TARGET_PORT 37001 //8008
#define MAXLINE 98

float timedifference_msec(struct timeval t0, struct timeval t1);

 int main(int argc, char *argv[]){
	int sock;
	int n;
	int i;
	int	len;
	int flags;
	int count = 0;
	float x;
	float y;
	char buffer[MAXLINE];
	char input [MAXLINE];
	//"67.11.114.92"
	char *str_addr = "67.11.114.92"; //127.0.0.1
	float elapsed;
	struct sockaddr_in     servaddr; 
	struct sockaddr_in     me;
	struct timeval t0;
    struct timeval t1;
	//str_addr = 67.11.114.92
	servaddr.sin_addr.s_addr = inet_addr(&str_addr[0]);
	 
	 /* create a socket to send on */
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error("Socket error");
	}   
	
	memset(&servaddr, 0, sizeof(servaddr));
	
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
	// start
	fprintf(stderr, "Simple UDP server process %d is ready!\n\n",(int)getpid());
    do {
		addrlen = sizeof(server);
		pid = fork(); /* if pid == 0, I am the child process I get time and compare*/
		if( pid == 0) {
			n = recvfrom(sock, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len);
			gettimeofday(&t1, 0);
			y = atof(buffer);
			elapsed += y - x;
			buffer[n] = '\0'; 
			printf("Server : %s\n", buffer);
			count++;
			//d0
		}else { /* I am the parent process send time */
			if(pid == -1) {
				perror("fork");
				return -1; 
			}
			gettimeofday(&t0, 0);
			x = ((float)(t0.tv_sec)*1000.0f + t0.tv_usec/ 1000.0f);
			printf("time now %f\n", x);
			gcvt(x, 25, input); 
			sendto(sock, (const char *)input, strlen(input), 
				flags, (const struct sockaddr *) &servaddr,  
					sizeof(servaddr)); 
			printf("Message sent.\n"); 
			//wait 2 seconds
			usleep(2000000);
			printf("Code executed in %f milliseconds.\n", elapsed);
		}
		if (count >= 5)
			break;
	} while(1);
	//end
	printf("average RTT = " elapsed/count );
	close(sock); 
    return 0; 
 }
 
 
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
