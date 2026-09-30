#include "utils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int write_all(int fd, const void *buf, size_t len) {
    const char *p = buf;
    size_t i = 0;
    while (i < len) {
        ssize_t batch = write(fd, p + i, len - i);
        if (batch == -1){
            if(errno == EINTR){
                continue;
            }
            return -1;
        }
        i += (size_t)batch;
    }
    return 0;
}

void reverse_string(char *str, size_t len) {
    if(len == 0){
        return;
    }
    size_t i = 0;
    size_t j = len - 1;
    while (i < j){
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++;
        j--;
    }
}
