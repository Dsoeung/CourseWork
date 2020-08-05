#include <unistd.h> // import library
#include <stdio.h>
void main()
{
// Creating first process
// when return value equal to 0 means process created successfully
// when value > 0 it means it's parent process
// here main is parent process and created process using fork are child
int n1 = fork();  
// Creating second process
int n2 = fork();
if (n1 > 0 && n2 > 0) {
printf("parent\n");
}
else if (n1 == 0 && n2 > 0)
{
printf("First Process\n");
}
else if (n1 > 0 && n2 == 0)
{
printf("Second Process\n");
}
}