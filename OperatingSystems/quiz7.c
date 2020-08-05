#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<pthread.h>
//#include<sys/wait.h>
int main(int argc, char** argv)
{
	int fd1[2];
	int fd2[2];
	int fd3[2];
	int fd4[2];
	char message[] = "abcd";
	char input_str[100];
	pid_t p,q;
	
	if (pipe(fd1)==-1)
	{
		fprintf(stderr, "Pipe Failed" );
		return 1;
		}
	
	if (pipe(fd2)==-1)
	{
		fprintf(stderr, "Pipe Failed" );
		return 1;
	}
	
	if (pipe(fd3)==-1)
	{
		fprintf(stderr, "Pipe Failed" );
		return 1;
	}
	
	if (pipe(fd4)==-1)
	{
		fprintf(stderr, "Pipe Failed" );
		return 1;
	}
	
	p = fork();
	
	if (p < 0)
	{
		fprintf(stderr, "fork Failed" );
		return 1;
	}
	// child process-1
	else if (p == 0)
	{
		close(fd1[0]);// Close reading end of first pipe
		char concat_str[100];
		printf("\n\tEnter meaaage:"):
		scanf("%s",concat_str);
		write(fd1[1], concat_str, strlen(concat_str)+1);
		// Concatenate a fixed string with it
		int k = strlen(concat_str);
		int i;
		for (i=0; i<strlen(fixed_str); i++)
		{
			concat_str[k++] = fixed_str[i];
		}
		concat_str[k] = '\0';//string ends with '\0'
		// Close both writting ends
		close(fd1[1]);
		wait(NULL);
		//.......................................................................
		close(fd2[1]);
		read(fd2[0], concat_str, 100);
		if(strcmp(concat_str,"invalid")==0)
		{
			printf("\n\tmessage not send");
		}
		else
		{
			printf("\n\tmessage send to prog_2(child_2).");
		}
		close(fd2[0]);//close reading end of pipe 2
		exit(0);
	}
	else
	{
		close(fd1[1]);//Close writting end of first pipe
		char concat_str[100];
		read(fd1[0], concal_str, strlen(concat_str)+1);
		close(fd1[0]);
		close(fd2[0]);//Close writing end of second pipe
		if(/*check if msg is valid or not*/)
		{
		//if not then
		write(fd2[1], "invalid",sizeof(concat_str));
		return 0;
		}
		else
		{
			//if yes then
			write(fd2[1], "valid",sizeof(concat_str));
			close(fd2[1]);
			q=fork();//create chile process 2
			if(q>0)
			{
				close(fd3[0]);/*close read head offd3[] */
				write(fd3[1],concat_str,sizeof(concat_str);//write message by monitor(main process) using fd3[1]
				close(fd3[1]);
				wait(NULL);//wait till child_process_2 send ACK
				//...........................................................
				close(fd4[1]);
				read(fd4[0],concat_str,100);
				close(fd4[0]);
				if(sctcmp(concat_str,"ack")==0)
				{
					printf("Messageof child process_1 is received by child process_2");
				}	
				else
				{
					printf("Messageof child process_1 is not received by child process_2");
				}
			}
			else
			{
				if(p<0)
				{
				printf("Chiile_Procrss_2 not cheated");
				}
				else
				{  
					close(fd3[1]);//Close writting end of first pipe
					char concat_str[100];
					read(fd3[0], concal_str, strlen(concat_str)+1);
					close(fd3[0]);
					close(fd4[0]);//Close writing end of second pipe
					write(fd4[1], "ack",sizeof(concat_str));
				}
			}
		}
  close(fd2[1]);
	}
}