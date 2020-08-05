#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
int main() {
	//if open is second offset is not shared so same base offset for read
    int fd;
    char buf[6] = "12345";
	fork();
    fd = open("tmpdata.txt", O_RDONLY);
    read(fd, buf, 2);
    read(fd, buf+2, 2);
    printf("%d: %s \n", getpid(), buf);
}