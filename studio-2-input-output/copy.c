#include <unistd.h>

#define bufferSize 200

char buffer[bufferSize];

int main(int argc, char *argv[]) {
    while (1) {
        ssize_t n = read(STDIN_FILENO, buffer, bufferSize);
        if (n <= 0) {
            break;
        }
        write(STDOUT_FILENO, buffer, n);
    }
    return 0;
}
