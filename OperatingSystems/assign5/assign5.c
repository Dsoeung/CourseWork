#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>
#define MAX 1024
/*Assign5 by Darin Soeung*/
//Define a very basic PCB structure
//Wanted to get to previous so I made PCB
//Into a Doubly Linked List
typedef struct PCB {
	int ProcId; 	
    int ProcPR; 
	int Reg[8]; 
    int numCPUBurst, numIOBurst; 
    int *CPUBurst, *IOBurst;
    int cpuindex, ioindex;
    int queueEnterClock;
    int waitingTime;
	 // more fields for performance measures
	// use the system time to determine how much waited etc
    struct PCB *prev, *next;
}PCB;

//global variables
pthread_mutex_t lock,lock2; 
sem_t cpuWait,ioWait;
char *scheduling;
char *inputFile;
int cpu_busy = 0,CLOCK;
int CLOCK;
int CPUreg[8];
int io_busy = 0;
int quantum; 
int Total_waiting_time;
int Total_turnaround_time;
int Total_job = 0,setp = 0;
int cpusum = 0;
int iosum = 0;
int ifile =0;
struct PCB *min, *max,*temp;
struct PCB *Head;
struct PCB *IOHead;
struct PCB *IOTail;
struct PCB *Tail;
struct timespec sec;


struct PCB *insertList(struct PCB *node,struct PCB *inHead,struct PCB *inTail) {
    char *IOburst;
    if(setp == 0){
        min = inHead;
        max = inHead;
    }
    if(inHead == Head){
        IOburst = "Head";
    }else{

        IOburst = "IOHead";
    }
    if (inHead == NULL) {
        inHead = node;
    if(setp == 0){
        min = inHead;
        max = inHead;
    }
        inTail = node;
        return inHead;
    }else{
        struct PCB* temp;
        temp = inHead;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = node;


    }
    return inHead;
}



int FIFO_Scheduling(int IOBurst) {
	int i = 0;
    int sleep;
    if (Head == NULL){ 
        return -1;
    }
    temp = Head;
    Head = Head->next;
	Total_job++;
    temp->next = NULL;
    temp->prev = NULL;
    sleep =  temp->CPUBurst[temp->cpuindex];
    CLOCK+= sleep;
    temp->cpuindex++;
	temp->waitingTime = temp->waitingTime + CLOCK - temp->queueEnterClock;
    if(i == 1)
    Total_waiting_time +=  CLOCK + temp->IOBurst[temp->ioindex -1];
    CLOCK += temp->CPUBurst[temp->cpuindex-1];
    Total_turnaround_time += CLOCK;
    usleep(sleep * 1000);
    if(temp->cpuindex >= temp->numCPUBurst ){
        if(temp==min)
            min = Head; 
        free(temp->CPUBurst);
        free(temp->IOBurst);
        free(temp);
        return -1;
    }
    IOHead = insertList(temp,IOHead,IOTail);
    return 1;
}

int RR_Scheduling() {

}

int SJF_Scheduling() {

}

int PR_Scheduling() {

}

/*chose the right alg to run*/
int CPUschedule(int IOBurst) {
	int i = 1;
	struct PCB *PCB;
    if(IOBurst == 1){
		if(IOHead == NULL){
			return 0;
		}
	PCB = IOHead;
    IOHead = IOHead->next;
    PCB->next = NULL;
    PCB->prev = NULL;
	PCB->waitingTime = PCB->waitingTime + CLOCK - PCB->queueEnterClock;
    if(i == 1)
    Total_waiting_time +=  CLOCK + PCB->IOBurst[PCB->ioindex -1];
    CLOCK += PCB->CPUBurst[PCB->cpuindex-1];
    Total_turnaround_time += CLOCK;
    int sleep = PCB->IOBurst[PCB->ioindex++];
    usleep(sleep * 1000);
    Head = insertList(PCB,Head,Tail);
    return 1;
    }
    if (strcmp(scheduling, "FIFO") == 0) {
        return FIFO_Scheduling(IOBurst);
    }
    if (strcmp(scheduling, "SJF") == 0) {
        return SJF_Scheduling();

    }
    if (strcmp(scheduling, "PR") == 0) {
        return PR_Scheduling();

    }
    if (strcmp(scheduling, "RR") == 0) {
        if (quantum == -1) {
            printf("Quantum Time was not set\n");
            printf("ERROR USAGE: assign5 -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [input_file_name.txt]\n");
            exit(-1);
        }
        return RR_Scheduling();
    }
}

struct PCB*newNode(int pr, int listsize, char *rest){
    struct PCB *temp;
	int x,y,z = 0;
    temp = malloc(sizeof(struct PCB));
    temp->ProcPR = pr; 
    temp->numCPUBurst = (listsize/2) + 1;
    temp->numIOBurst= listsize/2; 
    temp->CPUBurst = malloc(sizeof(int) * temp->numCPUBurst);
    temp->IOBurst = malloc(sizeof(int) * temp->numIOBurst);
    temp->cpuindex = 0; 
    temp->ioindex = 0;
    temp->queueEnterClock = 0;
    temp->waitingTime = 0;
	for(x = 0;x < listsize;x++){
        if(x%2 == 1){
            temp->IOBurst[y++] = atoi(strtok_r(rest," ",&rest));
            iosum+= temp->IOBurst[y-1];
        }else{
            temp->CPUBurst[z++] = atoi(strtok_r(rest," ",&rest));
            cpusum+= temp->CPUBurst[z-1];
        }
    }
    Head = insertList(temp,Head,Tail);
    setp++;
    return Head;
}


int FileReadThread(char *file){
    sec.tv_sec = 1;
    char buffer[MAX];
    FILE *open;
    char *str;
    int i =0;
    int pr,listsize;
	int x,y,z = 0;
	struct PCB *temp;
    open = fopen(file,"r");
    if( open == NULL){
        perror("Failed to Open file");
        exit(0);
    }

    while(fgets(buffer,sizeof(buffer),open) ){
        char *rest = buffer;
        str = strtok_r(rest," ",&rest);
        if( strcmp(str,"proc") == 0){
            pr = atoi(strtok_r(rest," ",&rest));
            listsize = atoi(strtok_r(rest," ",&rest));
			//
            newNode(pr,listsize,rest);
			//
            sem_post(&cpuWait);

        } 
        if(strcmp(str,"sleep") == 0 ){
            usleep(atoi(strtok_r(rest," ",&rest)) * 1000);
        }
        if(strcmp(str,"stop\n") == 0 || strcmp(str,"stop") == 0 ){
            ifile = 1;
            break;
        }
    }
    return 0;
}

int CPUThread(){
    int res,i =0;
    while(1){
        if(Head == NULL && IOHead == NULL&& ifile == 1 && io_busy == 0 ){
            break;
        }

        res = sem_timedwait(&cpuWait,&sec);
        if(res == -1 && errno==ETIMEDOUT ) continue ;
        cpu_busy = 1;

        pthread_mutex_lock(&lock);
        res =  CPUschedule(0);
        pthread_mutex_unlock(&lock);
        if (res == 1){
            sem_post(&ioWait);
        }

        cpu_busy = 0;
    }
}

int IO_sThread(){
    while(1){
		//if Ready_Q is empty and IO_Q is empty and file_read_done is 1, then break!
        if(Head == NULL && IOHead == NULL&& ifile == 1 && cpu_busy == 0){
            break;
        }
        int x = sem_timedwait(&ioWait,&sec);
        if(x == -1 && errno==ETIMEDOUT ) continue ;
        io_busy = 1;
        pthread_mutex_lock(&lock);
        CPUschedule(1);
        pthread_mutex_unlock(&lock);
        io_busy = 0;
        sem_post(&cpuWait);

    }
}

void FileThread(){
    FileReadThread(inputFile);
}

int main(int argc,char *argv[]) 
{
   int i = 1;
   if(argc < 2){
       printf("ERROR USAGE: assign5 -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [input_file_name.txt]\n");
       exit(0);
   }
   while (i != argc) {
       if (strcmp(argv[i], "-alg") == 0) {
           i++;
           if (strcmp(argv[i], "SJF") == 0) {
			   if (argc != 5) {
				printf("USAGE: prog -alg SJF -input fileName\n");
				return -1;
               }
               scheduling = argv[i];
           } else if (strcmp(argv[i], "PR") == 0) {
               scheduling = argv[i];
			   if (argc != 5) {
				printf("USAGE: prog -alg PR -input fileName\n");
				return -1;
               }
           } else if (strcmp(argv[i], "RR") == 0) {
               scheduling = argv[i];
			   if (argc != 5) {
				printf("USAGE: prog -alg RR -quantum [integer] -input fileName\n");
				return -1;
               }
           } else if (strcmp(argv[i], "FIFO") == 0) {
			   if (argc != 5) {
				printf("USAGE: prog -alg FIFO -input fileName\n");
				return -1;
               }
               scheduling = argv[i];
           } else {
               printf("ERROR USAGE: assign5 -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [input_file_name.txt]\n");
               exit(0);
           }
       }
       if (strcmp(argv[i], "-input") == 0) {
           i++;
           inputFile = argv[i];
       }
       if(strcmp(argv[i], "-quantum") == 0){
           i++;
           quantum = atoi(argv[i]);
       }
       i++;
   }
   sem_init(&cpuWait,0,0);
   sem_init(&ioWait,0,0);
   pthread_t mythread[3];
   pthread_mutex_init(&lock, NULL);
   pthread_mutex_init(&lock2, NULL);
   pthread_create(&mythread[0], NULL, (void *) FileThread, NULL);
   pthread_create(&mythread[1], NULL, (void *) CPUThread, NULL);
   pthread_create(&mythread[2], NULL,(void *) IO_sThread, NULL);
   pthread_join(mythread[0],NULL);
   pthread_join(mythread[1],NULL);
   pthread_join(mythread[2],NULL);
   printf("-------------------------------------------------------------------\n");
   printf("Input File Name				: %s\n", inputFile);
   printf("CPU Scheduling Alg			: %s\n", scheduling);
   printf("CPU utilization				: %.2f%%\n",(double) CLOCK/(CLOCK + iosum)*100);
   printf("Throughput				: %.3f jobs per ms\n", (double) Total_job / CLOCK);
   printf("Average Turnaround time			: %.2f ms\n", (double) iosum+cpusum / Total_job);
   printf("Average Waiting time in R queue\t	: %.2f ms\n", (double)  iosum / Total_job);
   printf("-------------------------------------------------------------------\n");
   return (EXIT_SUCCESS);

}
