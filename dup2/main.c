#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
int main() {
    int fd;

    fd = open("sample.txt", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("open() failed.");
        exit(EXIT_FAILURE);
    }

    dup2(fd, 1);
    printf("Hello World!\n");

    close(fd);

    return 0;
}
