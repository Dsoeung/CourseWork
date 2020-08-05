#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
int main() {
	//if open is first offset is shared so diff offset for read
    int fd;
    char buf[6] = "12345";
    fd = open("tmpdata.txt", O_RDONLY);
    fork();
    read(fd, buf, 2);
    read(fd, buf+2, 2);
    printf("%d: %s \n", getpid(), buf);
}