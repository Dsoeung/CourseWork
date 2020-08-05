#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//array for sorting!
struct sArray {
    double *dpArr;
	int iInput;
    int iStartingV;
    int iEndingV;
};

//array for merging!
struct mArray {
    double *dInArr; 
    double *dOutArr;
    int iInput;
    int iMid;
};

int swap(double dpArrA, double dpArrB);
int selectionSort(void *ptr);
int merge(void *ptr);

int main(int argc, char *argv[]) {
clock_t end;
clock_t start;
double * aArr; //A
double * bArr; //B
double * cArr; //C
int i;
int iMid;
int iNput;
pthread_t thA1; //thA1
pthread_t thA2; //thA2
pthread_t thB;  //thB
pthread_t thM; //thM
struct sArray aFirstHalf;
struct sArray aSecondHalf;
struct mArray mThread;

if(argc != 2) {
	//error check number of args
      printf("ERROR USAGE: %s (Integer) \n", argv[0]);
      return -1;
}

//iNput will be given as command line argument
iNput = atoi(argv[1]);

//create array A ( n double values) and randomly generate these values 
aArr = (double*)malloc(iNput * sizeof(double));
//B same size of A
bArr = (double*)malloc(iNput * sizeof(double));
//C same size of A
cArr = (double*)malloc(iNput * sizeof(double));

srand(time(NULL));

for(i = 0; i < iNput; i++) {
    aArr[i] = (double)rand() / RAND_MAX * 99.0 + 1.0; /* range 1.0 - 100.0 */
}

//Afirsthalf  and AsecondHalf with the half size of A

//copy A into Afirsthalf  and AsecondHalf

//Afirsthalf
aFirstHalf.iStartingV = 0;
aFirstHalf.iEndingV = iNput/2;
aFirstHalf.dpArr = aArr;

//AsecondHalf
aSecondHalf.iStartingV = iNput/2;
aSecondHalf.iEndingV = iNput; //iNput being TOTAL value
aSecondHalf.dpArr = aArr;

//copy A into B
for(i = 0; i < iNput; i++) {
	aArr[i] = bArr[i];
}


//create thM  mergeThread to merge Afirsthalf and Asecondhalf into C
mThread.iMid = iNput/2;
mThread.iInput = iNput;
mThread.dInArr = aArr;
mThread.dOutArr = cArr;

//doing two threads first for the sake of the example output

start = clock();

//int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
//                        void *(*start_routine) (void *), void *arg);

//create thA1  sortThread to sort Afirsthalf   /* use selection/insertion sort O((n/2)^2) */
pthread_create(&thA1, NULL, selectionSort, &aFirstHalf);
//create thA2  sortThread to sort Asecondhalf    /* use selection/insertion sort O((n/2)^2) */
pthread_create(&thA2, NULL, selectionSort, &aSecondHalf);


//join thA1
//int pthread_join(pthread_t thread, void **retval);
pthread_join(thA1, NULL);
//join thA2
pthread_join(thA2, NULL);

//create thM  mergeThread to merge Afirsthalf and Asecondhalf into C

pthread_create(&thM, NULL, merge, &mThread);
//join thM
pthread_join(thM, NULL);

//Sorting is done in (end - start) ms when two threads are used 
end = clock() - start;

printf("Sorting is done in %f ms when two threads are used\n", end * 1000.0f / CLOCKS_PER_SEC);

//Time for single thread
aFirstHalf.iEndingV = iNput;
aFirstHalf.dpArr = bArr; 
start = clock();

//create thB sortThread to sort B
pthread_create(&thB, NULL, selectionSort, &aFirstHalf);
selectionSort(&aFirstHalf);
pthread_join(thB);


//Sorting is done in (end - start) ms when one thread is used 
end = clock() - start; 

printf("Sorting is done in %f ms when one thread is used\n", end * 1000.0f / CLOCKS_PER_SEC);

free(aArr);
free(bArr);
free(cArr);
return 0;
}

int selectionSort(void *ptr) {
int i;
int j;
int minIdx;
double tmp;
double *dpArr;
struct sArray* p = (struct sArray*) ptr;
dpArr = p->dpArr;
for(i = p->iStartingV; i < p->iEndingV; i++) {
    minIdx = i;
    for(j = i + 1; j < p->iEndingV; j++) {
      if(dpArr[j] < dpArr[minIdx])
        minIdx = j;
    }
	//Swap
	swap(dpArr[i], dpArr[minIdx]);
}
return 0;
}

int swap(double dpArrA, double dpArrB) {
	double tmp = dpArrA;
	dpArrA = dpArrB;
	dpArrB = tmp;
	return 0;
}

int merge(void *ptr) {
double *in;
double *out;
int i = 0;
int j;
int k = 0;
struct mArray* p = (struct mArray*) ptr;
j = p->iMid;
in = p->dInArr;
out = p->dOutArr;
while(i < p->iMid && j < p->iInput) {
    if(in[i] < in[j])
      out[k++] = in[i++];
    else
      out[k++] = in[j++];
}
while(i < p->iMid)
    out[k++] = in[i++];
while(j < p->iInput)
    out[k++] = in[j++];
return 0;
}