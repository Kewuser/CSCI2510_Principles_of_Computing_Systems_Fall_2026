#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    if (pipe(fd) == -1) { perror("pipe"); return 1; }

    pid_t p1 = fork();
    if (p1 == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        char* cmd = "./print";
        char* myargv[] = {"print", NULL};
        int ret = execvp(cmd, myargv);
        if (ret == -1) perror("Error exec'ing print");
        exit(1);
    }

    pid_t p2 = fork();
    if (p2 == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        char* cmd = "./echo";
        char* myargv[] = {"echo", NULL};
        int ret = execvp(cmd, myargv);
        if (ret == -1) perror("Error exec'ing echo");
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    return 0;
}
