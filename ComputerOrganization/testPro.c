#include<stdio.h>
#include <stdlib.h>
int power(int x, int i){
    int t = 1;
    int result = 1;
    if (i != 0){
    for(t=1;t<=i;t++){
        result*=x;
        }
    }
    return result;
}

void fillArray(int x, int a[],int n){
    int temp = 0;
    int result = x;
    for (temp = 0;temp<n;temp++){
        result = power(x,temp);
        a[temp] = result;
    }
}

void fillArray2(int x, int a[],int n){
    int temp = 1;
    int t = 0;
    int result = 1;
    for (temp = 0;temp<=n;temp++){
        for(t=0;t<temp;t++){
            result*=x;
    }
        a[temp] = result;
        result = 1;
    }
}

int compare(int a[], int b[], int n){
    int tf;
    int i = 0;
    for (i=0; i<n; i++) 
         if (a[i] != b[i]) 
            return tf = 0; 
    return tf = 1; 
}

int main() {
    int *array = malloc(10*sizeof(int));
    int a[10];
    int i;
    int x = 2;
    int y = 4;
    int re = power(324,0);
   fillArray(2,array,10);
   for (i = 0; i < 10; i ++){
            printf( "%d\n", array[i]) ;
    }
   fillArray2(3,array,10);
   for (i = 0; i < 10; i ++){
            printf( "%d\n", array[i]) ;
    }
}