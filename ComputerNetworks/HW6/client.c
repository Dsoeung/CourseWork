//Darin Soeung
#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>	

#define TARGET_PORT 37001 //8008
#define MAXLINE 98

float timedifference_msec(struct timeval t0, struct timeval t1);
double packets;
double tTotal;
int sock;
int n;
int i;
int rCount;
int	len;
int flags;
int count;
float x;
float y;
char buffer[MAXLINE];
char input [MAXLINE];
//"67.11.114.92"
char *str_addr = "67.11.114.92"; //127.0.0.1
float elapsed;
pid_t pid;

struct sockaddr_in     servaddr; 
struct sockaddr_in     me;
struct timeval t0;
struct timeval t1;

float timedifference_msec(struct timeval t0, struct timeval t1);
void * recFunc();

 int main(int argc, char *argv[]){
	
	int rc = 0;
	pthread_t * pThreads;
	
	//str_addr = 67.11.114.92
	servaddr.sin_addr.s_addr = inet_addr(&str_addr[0]);
	 
	 /* create a socket to send on */
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error("Socket error");
	}   
	
	char test[MAXLINE];
		memset(&servaddr, 0, sizeof(servaddr));
	
	// Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(TARGET_PORT); //8080
    servaddr.sin_addr.s_addr = inet_addr(str_addr);	//127.0.0.1
	
	me.sin_family = AF_INET;
	me.sin_port = htons(0);
	me.sin_addr.s_addr = INADDR_ANY;
	
	i = bind(sock, (struct sockaddr *) &me, sizeof(me));
	printf("Enter amount of messages you want to send: ");
    scanf("%d", &count);
	
	if( i < 0) {
	printf("bind result: %d\n", i);
	return -1;
	}
	pThreads = malloc(count*sizeof(pthread_t));
	for( i = 0; i < count; i++) {
		gettimeofday(&t0, NULL);
		float x = ((float)(t0.tv_sec)*1000.0f + t0.tv_usec/ 1000.0f);
		gcvt(x, 6, test);

		if( sendto( sock, (char *) test, sizeof(test), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) > 0) {
			// int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                        //void *(*start_routine) (void *), void *arg);
			//start routine being each recvfrom
			rc = pthread_create( &pThreads[i], NULL, recFunc, NULL);
			if(rc) {
				error("Error creating thread");
			}
		} else {
			printf("No bytes were written :(\n");
		}
		//sleep for 2 seconds
		usleep(2000000);
	}
	printf("Percent Loss = %f%%\n", (float)(100-((packets/count)*100)));
	printf("Average RTT: %.2fms\n", (tTotal/1000)/packets);
	free(pThreads);
	close(sock);
	return(0);
 }
 
 void * recFunc()
{
	if( recvfrom(sock, buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len) > 0 ){
		//take time when message recieved
		float testF;
		gettimeofday(&t1, NULL);
		printf("Packet recieved\n");
		//number we get back
		printf("Server msg [ %s ]\n", buffer);
		testF = atof(buffer);
		packets++;
		tTotal += (t1.tv_usec - t0.tv_usec);
	} 
	pthread_exit(NULL);
}
 
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
