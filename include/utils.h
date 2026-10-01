#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <sys/types.h>

#define LENGTH_THRESHOLD 10

void die(const char *msg);

int write_all(int fd, const void *buf, size_t len);

void reverse_string(char *str, size_t len);

size_t utf8_length(const char *str, size_t len);

#endif
