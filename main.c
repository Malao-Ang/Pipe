#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    int fd1[2];
    int fd2[2];
    pid_t pid;
    char string[] = "Hello Operating system ";
    char buffer[100];

    if (pipe(fd1) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* Child process */
        close(fd1[1]);
        close(fd2[0]);
        read(fd1[0], buffer, sizeof(buffer));
        int i;
        for (i = 0; buffer[i]; i++) {
            buffer[i] = toupper(buffer[i]);
        }
        write(fd2[1], buffer, strlen(buffer));
        close(fd1[0]);
        close(fd2[1]);
        exit(EXIT_SUCCESS);
    }
    else {
        /* Parent process */
        close(fd1[0]);
        close(fd2[1]);
        write(fd1[1], string, strlen(string));
        read(fd2[0], buffer, sizeof(buffer));
        printf("Answer : %s", buffer);
        close(fd1[1]);
        close(fd2[0]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
