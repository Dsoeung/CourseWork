#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define FIFO 1
#define SJF  2
#define PR  3
#define RR  4



//Define a very basic PCB structure
//Wanted to get to previous so I made PCB
//Into a Doubly Linked List
typedef struct PCB_st {
   int ProcId; 
   int ProcPR; 
   int CPUburst; 
   int Reg[8]; 
   int queueEnterClock, waitingTime; 
   /* other info */ 
   struct PCB_st *prev;
   struct PCB_st *next;
}PCB_st;

//Functs
int FIFO_Scheduling(PCB_st *PCB);
int SJF_Scheduling(PCB_st *Head, PCB_st *Tail);
int PR_Scheduling(PCB_st *Head, PCB_st *Tail);
//int RR_Scheduling(PCB_st *pPCB, int quantum);

struct PCB_st *Head=NULL;
struct PCB_st *Tail=NULL;

//basic CPU 8 registers
//Global Variables
int CPUreg[8] = {0};
int CLOCK=0; 
int Total_waiting_time=0; 
int Total_turnaround_time=0; 
int Total_job=0;


int main(int argc, char *argv[]){
   FILE *pInFile;
   int i;
   int iFlag;
   int iCpuBurstT;
   int iPId;
   int iPPrio;
   int iQuantum = 0;
   int j;
	
   //Check for scheduling algorithm first
   if (strcmp(argv[2], "FIFO") == 0) {
      if (argc != 5) {
		 //Error check number of arguments
         printf("USAGE: prog -alg FIFO -input fileName\n");
         return -1;
		 }
      iFlag = FIFO;
      } else if (strcmp(argv[2], "SJF") == 0) {
      if (argc != 5) {
         printf("USAGE: prog -alg SJF -input fileName\n");
		 return -1;
      } 
		iFlag = SJF;
	}else if (strcmp(argv[2], "PR") == 0) {
      if (argc != 5) {
         printf("USAGE: prog -alg PR -input fileName\n");
		 return -1;
      } 
		iFlag = PR;
	}else if (strcmp(argv[2], "RR") == 0) {
      if (argc != 5) {
         printf("USAGE: prog -alg RR -quantum [integer] -input fileName\n");
		 return -1;
      } 
		iFlag = RR;
	}
	
   // Open the input file, "r"
   pInFile = fopen(argv[4], "r");

   // Loop through the file line by line and read in contents to a PCB_st
   while(fscanf(pInFile, "%d %d %d\n", &iPId, &iPPrio, &iCpuBurstT) != EOF) {
      // dynamically create a struct PCB_st pointed by PCB,
	  struct PCB_st *pPCBst;
	  pPCBst = malloc(sizeof(struct PCB_st));
	  if(pPCBst == NULL) {
         printf("Failed in allocating pPCBst. \n");
      }
      else {
         //save the given data into correponding fields of PCB,
         pPCBst->ProcId = iPId;
         pPCBst->ProcPR = iPPrio;
		 pPCBst->CPUburst = iCpuBurstT;
		 
        
		 // set all PCB->Reg[8] to the Process ID, 
		 for(j = 0; j < 8; j++) {
         pPCBst->Reg[j] = iPId;
         }
			
         pPCBst->prev = NULL;
		 pPCBst->next = NULL;
			
           
		   // set PCB->queueEnterClock and PCB->waitingTime to 0, then
		   
         pPCBst->queueEnterClock = 0;
		 pPCBst->waitingTime = 0;
		   
          
		  // insert this PCB at the end of the link list.
         if(Tail == NULL) {
            Tail = pPCBst;
         }
         if(Head == NULL) {
            Head = pPCBst;
		 }
         if(Head != NULL) {
            Tail->next = pPCBst;
            Tail->next->prev = Tail;
         }
            Tail = pPCBst;
      }
   }

   // Close the file and print correct fields
   fclose(pInFile);

   //Switch statement when picking scheduling algorithm
   switch (iFlag) {
      case FIFO:
         printf("Student Name : Darin Soeung\n");
         printf("Scheduling Algorithm : %s\n\n", argv[2]);
		 printf("Input File Name : %s\n", argv[4]);
		 FIFO_Scheduling(Head);
		 break;
      case SJF:
         printf("Student Name : Darin Soeung\n");
		 printf("Scheduling Algorithm : %s\n\n", argv[2]);
		 printf("Input File Name : %s\n", argv[4]);
		 SJF_Scheduling(Head, Tail);
		 break;
      case PR:
         printf("Student Name : Darin Soeung\n");
		 printf("Scheduling Algorithm : %s\n\n", argv[2]);
		 printf("Input File Name : %s\n", argv[4]);
		 PR_Scheduling(Head, Tail);
      case RR:
         printf("Student Name : Darin Soeung\n");
		 printf("Scheduling Algorithm : %s\n\n", argv[2]);
		 printf("Input File Name : %s\n", argv[4]);
		 //-alg RR option is given along with -quantum integer(ms).
		 //RR_Scheduling();
   }
	
   fflush(stderr);
   return 0;
}

int FIFO_Scheduling(PCB_st *pPCB){
   struct PCB_st *pTempPCB = NULL;
   int iPCB = 1;
   int i;
   
   // performs the followings until the linked list is empty: 
   while (pPCB != NULL) {
	   
      // Do context-switching:
      for (i = 0; i < 8; i++) {
      CPUreg[i] = pPCB->Reg[i];
      CPUreg[i]++;
      pPCB->Reg[i] = CPUreg[i];
      }

      // Data collection for performance metrics
      pPCB->waitingTime = pPCB->waitingTime + CLOCK - pPCB->queueEnterClock;
      Total_waiting_time = Total_waiting_time + pPCB->waitingTime;
      CLOCK = CLOCK + pPCB->CPUburst;
	  Total_turnaround_time = Total_turnaround_time + CLOCK;
	  Total_job = Total_job + 1;

      // Print process number and time to completion
      printf("Process %d is completed at %d ms\n", iPCB, CLOCK);

      // Continue to next node and free the last node
      iPCB++;
      pTempPCB = pPCB->next;
      // Free pPCB. Since there is no more CPUburst or I/O burst
      // this process is terminated here! 
      free(pPCB);
      pPCB = pTempPCB;
	}

   // print the perfromance metrics mentioned
   printf("\nAverage Waiting Time: %.2f per ms	(%d / %d)\n",
         (double)Total_waiting_time/(double)Total_job,Total_waiting_time,Total_job);
   printf("Average Turnaround Time: %.2f per ms	(%d / %d)\n", 
         (double)Total_turnaround_time/(double)Total_job,Total_turnaround_time,Total_job);
   printf("Throughput: %.2f per ms			(%d / %d)\n", 
         (double)Total_job/(double)CLOCK, Total_job, CLOCK);
   return 0;
	
}

int SJF_Scheduling(PCB_st *Head, PCB_st *Tail) {
   int iCPUburst = 0;
   int iProcId = 0;
   struct PCB_st *pHead = Head;
   struct PCB_st *pTail = Tail;
   struct PCB_st *pTemp;
   pTail->next = pHead;
   // contextSwitching(pHead);
   while(pHead->next != NULL){
      if(iCPUburst == 0){
      iCPUburst = pHead->CPUburst;
      iProcId = pHead->ProcId;
   } else {
   if(pHead->CPUburst < iCPUburst){
     iCPUburst = pHead->CPUburst;
     iProcId = pHead->ProcId;
   } else
      if(pHead->CPUburst == iCPUburst && pHead->ProcId == iProcId){
	  // Data collection for performance metrics
	  pHead->waitingTime = pHead->waitingTime + CLOCK - pHead->queueEnterClock;
	  Total_waiting_time = Total_waiting_time + pHead->waitingTime ;
	  CLOCK = CLOCK + pHead->CPUburst;
	  Total_turnaround_time = Total_turnaround_time + CLOCK;
	  Total_job = Total_job + 1;
	  
	  // Continue to next node and free the last node
	  printf("Process %d is completed at %d ms\n", pHead->ProcId,CLOCK);
	  pTemp->next = pHead->next;
	  iCPUburst = 0;
	  iProcId = 0;
        if (pTemp->next->ProcId == pHead->ProcId) {
          break;
        }
      }
   }
    pTemp = pHead;
    pHead = pHead->next;
  }
	printf("\n");
   // print the perfromance metrics mentioned
   printf("\nAverage Waiting Time: %.2f per ms	(%d / %d)\n",
         (double)Total_waiting_time/(double)Total_job,Total_waiting_time,Total_job);
   printf("Average Turnaround Time: %.2f per ms	(%d / %d)\n", 
         (double)Total_turnaround_time/(double)Total_job,Total_turnaround_time,Total_job);
   printf("Throughput: %.2f per ms			(%d / %d)\n", 
         (double)Total_job/(double)CLOCK, Total_job, CLOCK);
   return 0;
}

int PR_Scheduling(PCB_st *Head, PCB_st *Tail) {
   int iProcPr = 0;
   int iProcId = 0;
   int iCount;
   struct PCB_st *pHead = Head;
   struct PCB_st *pTail = Tail;
   struct PCB_st *pTemp = Tail;
   pTail->next = pHead;
  
   while(pHead->next != NULL){
    if(iProcPr == 0){
    iProcPr = pHead->ProcPR;
    iProcId = pHead->ProcId;
   }
   if(pHead->ProcPR > iProcPr){
      iProcPr = pHead->ProcPR;
      iProcId = pHead->ProcId;
      }else
   if(pHead->ProcPR == iProcPr && pHead->ProcId == iProcId){    
      iCount++;
	  // Data collection for performance metrics
	  pHead->waitingTime = pHead->waitingTime + CLOCK - pHead->queueEnterClock;
	  Total_waiting_time = Total_waiting_time + pHead->waitingTime ;
	  CLOCK = CLOCK + pHead->ProcPR;
	  Total_turnaround_time = Total_turnaround_time + CLOCK;
	  Total_job = Total_job + 1;
	  
	  // Continue to next node and free the last node
	  printf("Process %d is completed at %d ms\n", pHead->ProcId,CLOCK);
	  pTemp->next = pHead->next;
	  iProcPr = 0;
	  iProcId = 0;
	  if (pTemp->next->ProcId == pHead->ProcId) {
          break;
      }
   }
   pTemp = pHead;
   pHead = pHead->next;
  }
	printf("\n");
   // print the perfromance metrics mentioned
   printf("\nAverage Waiting Time: %.2f per ms	(%d / %d)\n",
         (double)Total_waiting_time/(double)Total_job,Total_waiting_time,Total_job);
   printf("Average Turnaround Time: %.2f per ms	(%d / %d)\n", 
         (double)Total_turnaround_time/(double)Total_job,Total_turnaround_time,Total_job);
   printf("Throughput: %.2f per ms			(%d / %d)\n", 
         (double)Total_job/(double)CLOCK, Total_job, CLOCK);
   return 0;
}

//-alg RR option is given along with -quantum integer(ms).
int RR_Scheduling(PCB_st *pPCB, int quantum) {
    int i;
	int iPCB;
    PCB_st *pTempPCB = NULL;
	
	while (pPCB != NULL) {
	   
      // Do context-switching:
      for (i = 0; i < 8; i++) {
      CPUreg[i] = pPCB->Reg[i];
      CPUreg[i]++;
      pPCB->Reg[i] = CPUreg[i];
      }

      // Data collection for performance metrics
      pPCB->waitingTime = pPCB->waitingTime + CLOCK - pPCB->queueEnterClock;
      Total_waiting_time = Total_waiting_time + pPCB->waitingTime;
      CLOCK = CLOCK + pPCB->CPUburst;
	  Total_turnaround_time = Total_turnaround_time + CLOCK;
	  Total_job = Total_job + 1;

      // Print process number and time to completion
      printf("Process %d is completed at %d ms\n", iPCB, CLOCK);

      // Continue to next node and free the last node
      iPCB++;
      pTempPCB = pPCB->next;
      // Free pPCB. Since there is no more CPUburst or I/O burst
      // this process is terminated here! 
      free(pPCB);
      pPCB = pTempPCB;
	}
	
   // print the perfromance metrics mentioned
   printf("\nAverage Waiting Time: %.2f per ms	(%d / %d)\n",
         (double)Total_waiting_time/(double)Total_job,Total_waiting_time,Total_job);
   printf("Average Turnaround Time: %.2f per ms	(%d / %d)\n", 
         (double)Total_turnaround_time/(double)Total_job,Total_turnaround_time,Total_job);
   printf("Throughput: %.2f per ms			(%d / %d)\n", 
         (double)Total_job/(double)CLOCK, Total_job, CLOCK);
   return 0;
}





















