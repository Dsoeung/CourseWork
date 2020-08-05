/* server.c - code for example server program that uses TCP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct student_info {
	char abc123[7];
	char name[10];
	double GPA;
}students[10];

struct student_info students[] = { 
    {"iou957", "Darin", 4.0}, 
    {"xdf123", "John", 3.5} ,
    {"fas516", "Ben", 2.5} ,
    {"bea628", "Fas", 1.5} 
}; 

void printOut(int sockfd);

main(argc, argv)
     int     argc;
     char    *argv[];
{
  struct  sockaddr_in sad; /* structure to hold server's address  */
  int     port;            /* protocol port number                */
  
  struct  sockaddr_in cad; /* structure to hold client's address  */
  int     alen;            /* length of address                   */
  
  int     welcomeSocket, connectionSocket; /* socket descriptors  */
  
  char    clientSentence[128]; 
  char    capitalizedSentence[128]; 
  char    buff[128];
  int     i, n, pid;
  
  /* Check command-line argument for protocol port and extract   */
  /* port number if one is specified. Otherwise, give error      */
  if (argc > 1) {                /* if argument specified        */
    port = atoi(argv[1]);        /* convert argument to binary   */
  } else { 
    fprintf(stderr,"Usage: %s port-number\n",argv[0]);
    exit(1);
  }
  
  /* Create a socket */

  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0); /* CREATE SOCKET */
  if (welcomeSocket < 0) {
    fprintf(stderr, "socket creation failed\n");
    exit(1);
  }
  
  /* Bind a local address to the socket */
  
  memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure   */
  sad.sin_family = AF_INET;           /* set family to Internet     */
  sad.sin_addr.s_addr = INADDR_ANY;   /* set the local IP address   */
  sad.sin_port = htons((u_short)port);/* set the port number        */ 
  
  if (bind(welcomeSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    fprintf(stderr,"bind failed\n");
    exit(1);
  }
  
  /* Specify the size of request queue */
  
  if (listen(welcomeSocket, 10) < 0) {
    fprintf(stderr,"listen failed\n");
    exit(1);
  }
  
  /* Main server loop - accept and handle requests */
  
  while(1)
  {
	printf("\nServer: Starting accept");
	if((connectionSocket = accept(welcomeSocket,(struct sockaddr *)&cad, &alen)) == -1){
		fprintf(stderr, "accept failed\n");
		exit(1);
	}
	pid = fork();
	if(pid < 0){
		fprintf(stderr, "Error in Fork\n");
		fflush(stdout);
		exit(0);
	}
	if(pid == 0){
		//CHILD PROCESS
		close(welcomeSocket);
		printOut(connectionSocket);
		fprintf(stderr, "Closing server connection\n");
		exit(0);
	}
	else{
		close(connectionSocket);
	}
  }
}

void printOut(int sockfd){
char buffer[30];
int i, x, flag = 0;
char *p;
 
//printf("%s \n",students[1].abc123);

for (x = 0; x < 30; x++){
	buffer[x] = '\0';
}

recv(sockfd, buffer, 30, 0);

p = strtok (buffer," ");


	//The server (child process part) then waits for the next query until it gets “STOP” msg.
if(strcmp("STOP", p) == 0){
	//both the client and the server 
	//(child process part) close sockets and terminate.
	close(sockfd);
	return;
}

//IF NOT STOP then look for "GETGPA abc123"
if(strcmp("GETGPA", p) == 0){
	printf("\nSelected GETGPA : \n");
	//get abc123
	//P = GETGPA
	p = strtok (NULL, " ");
	//P = abc123
	//search for abc123 of the student in the array
	for(i = 0; i<10; i++){
		//parse through our list and search for the student
		if(strcmp(students[i].abc123, p) == 0){
			printf("\nStudent found");
			fflush(stdout);
			for (x = 0; x < 30; x++){
				buffer[x] = '\0';
			}
			sprintf(buffer, "%s %f", students[i].name, students[i].GPA);
			send(sockfd, buffer, strlen(buffer), 0);
			flag = 1;
		}	
	}

	if(flag != 1){
		printf("\nStudent not found!");
		fflush(stdout);
		for (x = 0; x < 30; x++){
			buffer[x] = '\0';
		}
		strcpy(buffer, "Student information not found!");
		send(sockfd, buffer, strlen(buffer), 0);
	}
}
close(sockfd);
}




