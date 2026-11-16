#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h>

char alphabet[128];
int alphabet_size;

typedef struct {
    char salt[3];
    char target[32];
    int len;
    int start_char;
    int num_chars;
} thread_args;

void *worker(void *arg) {
    thread_args *a = (thread_args *)arg;
    struct crypt_data data;
    data.initialized = 0;

    long rest_total = 1;
    for (int i = 0; i < a->len - 1; i++) rest_total *= alphabet_size;

    char buf[9];
    for (int c = a->start_char; c < a->start_char + a->num_chars; c++) {
        buf[0] = alphabet[c];
        for (long n = 0; n < rest_total; n++) {
            long x = n;
            for (int i = a->len - 1; i >= 1; i--) {
                buf[i] = alphabet[x % alphabet_size];
                x /= alphabet_size;
            }
            buf[a->len] = '\0';

            char *hash = crypt_r(buf, a->salt, &data);
            if (hash != NULL && strcmp(hash, a->target) == 0) {
                printf("%s\n", buf);
                fflush(stdout);
                exit(0);
            }
        }
    }
    free(a);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5) {
        printf("Usage: crack <threads> <keysize> <target> [extended]\n");
        return -1;
    }

    int nthreads = atoi(argv[1]);
    int keysize = atoi(argv[2]);
    char *target = argv[3];
    int extended = (argc == 5);

    if (nthreads < 1) nthreads = 1;

    if (extended) {
        alphabet_size = 0;
        for (int c = 33; c <= 126; c++) {
            alphabet[alphabet_size++] = (char)c;
        }
    } else {
        alphabet_size = 26;
        for (int i = 0; i < 26; i++) {
            alphabet[i] = 'a' + i;
        }
    }

    for (int len = 1; len <= keysize; len++) {
        int actual_threads = nthreads < alphabet_size ? nthreads : alphabet_size;

        pthread_t threads[actual_threads];
        int base = alphabet_size / actual_threads;
        int remainder = alphabet_size % actual_threads;
        int start = 0;

        for (int i = 0; i < actual_threads; i++) {
            thread_args *a = malloc(sizeof(thread_args));
            strncpy(a->salt, target, 2);
            a->salt[2] = '\0';
            strncpy(a->target, target, 31);
            a->target[31] = '\0';
            a->len = len;
            a->num_chars = base + (i < remainder ? 1 : 0);
            a->start_char = start;
            start += a->num_chars;

            pthread_create(&threads[i], NULL, worker, a);
        }

        for (int i = 0; i < actual_threads; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    return 0;
}