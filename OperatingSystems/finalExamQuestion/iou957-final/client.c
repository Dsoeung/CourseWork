/* client.c - code for example client program that uses TCP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

void catcher(int signum)
{
    switch (signum) {
    case SIGINT:
        fprintf(stderr, "CTRL-C was pressed! \n");
		printf("Sentence   :   \n"); 
        return;
    default:
        fprintf(stderr, "Caught a signal.\n");
        return;
    }
}

int install_catcher(const int signum)
{
    struct sigaction  act;

    memset(&act, 0, sizeof act);
    sigemptyset(&act.sa_mask);

    act.sa_handler = catcher;
    act.sa_flags = SA_RESTART;  /* Do not interrupt "slow" functions */
    if (sigaction(signum, &act, NULL) == -1)
        return -1;  /* Failed */

    return 0;
}



main(argc, argv)
     int     argc;
     char    *argv[];
{ 
  struct  sockaddr_in sad; /* structure to hold an IP address     */
  int     clientSocket;    /* socket descriptor                   */ 
  struct  hostent  *ptrh;  /* pointer to a host table entry       */
  
  char    *host;           /* pointer to host name                */
  int     port;            /* protocol port number                */  
  
  char    Sentence[128]; 
  char    modifiedSentence[128]; 
  char    buff[128];
  int     n;
  
  struct sigaction sigIntHandler;
  
  if (argc != 3) {
    fprintf(stderr,"Usage: %s server-name port-number\n",argv[0]);
    exit(1);
  }
  
  /* Extract host-name from command-line argument */
  host = argv[1];         /* if host argument specified   */
  
  /* Extract port number  from command-line argument */
  port = atoi(argv[2]);   /* convert to binary            */
  
  /* Create a socket. */
  
  
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    fprintf(stderr, "socket creation failed\n");
    exit(1);
  }
  
  

  /* Connect the socket to the specified server. */

  memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
  sad.sin_family = AF_INET;           /* set family to Internet     */
  sad.sin_port = htons((u_short)port);
  ptrh = gethostbyname(host); /* Convert host name to equivalent IP address and copy to sad. */
  if ( ((char *)ptrh) == NULL ) {
    fprintf(stderr,"invalid host: %s\n", host);
    exit(1);
  }
  memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);
  
  printf("Client is waiting for CTRL-C. \n");
  if (install_catcher(SIGINT)) {
        fprintf(stderr, "Cannot install signal handler: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
  
  if (connect(clientSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    fprintf(stderr,"connect failed\n");
    exit(1);
  }
  
  /* Read a sentence from user */
  
  
  fgets(Sentence, sizeof(Sentence), stdin);
  Sentence[strlen(Sentence) -1] = '\0';
  
  /* Send the sentence to the server  */
  
  write(clientSocket, Sentence, strlen(Sentence)+1);
  
  /* Get the modified sentence from the server and write it to the screen*/
  modifiedSentence[0]='\0';
  n=read(clientSocket, buff, sizeof(buff));
  while(n > 0){
    strncat(modifiedSentence,buff,n);
    if (buff[n-1]=='\0') break;
    n=read(clientSocket, buff, sizeof(buff));
  }
  
  printf("Modified to -> %s\n",modifiedSentence);

  /* Close the socket. */
  
  close(clientSocket);
  
}


