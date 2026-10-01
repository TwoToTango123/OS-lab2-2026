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

static int is_utf8_continuation(char c) {
    return ((unsigned char)c & 0xC0) == 0x80;
}

static void reverse_bytes(char *str, size_t len) {
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

void reverse_string(char *str, size_t len) {
    size_t i = 0;
    while (i < len) {
        size_t j = i + 1;
        while (j < len && is_utf8_continuation(str[j])) {
            j++;
        }
        reverse_bytes(str + i, j - i);
        i = j;
    }
    reverse_bytes(str, len);
}

size_t utf8_length(const char *str, size_t len) {
    size_t count = 0;
    for (size_t i = 0; i < len; i++) {
        if (!is_utf8_continuation(str[i])) {
            count++;
        }
    }
    return count;
}
