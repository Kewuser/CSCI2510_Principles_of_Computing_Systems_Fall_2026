#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char* child_argv[] = {"ls", "-l", NULL};
    char* child_prog = child_argv[0];

    pid_t child_PID = fork();

    if (child_PID == 0) {
        execvp(child_prog, child_argv);
    } else {
        waitpid(child_PID, NULL, 0);
        printf("Parent: PID %d, child PID %d\n", getpid(), child_PID);
    }
    return 0;
}
