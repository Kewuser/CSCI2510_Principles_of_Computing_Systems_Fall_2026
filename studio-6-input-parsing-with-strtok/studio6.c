#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    char buffer[256];

    int max_args = 15;
    int max_argv_size = max_args + 2; //one for argv[0], one for null terminator
    char* cmd;
    char* my_argv[max_argv_size];

    printf("Enter input: ");
    fgets(buffer, sizeof(buffer), stdin);

    buffer[strcspn(buffer, "\n")] = '\0';

    cmd = strtok(buffer, " ");
    my_argv[0] = cmd;

    int i = 1;
    char* res = strtok(NULL, " ");
    while (res != NULL && i < max_args + 1) {
        my_argv[i] = res;
        i++;
        res = strtok(NULL, " ");
    }
    my_argv[i] = NULL;

    execvp(cmd, my_argv);

    return 0;
}
